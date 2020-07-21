#include <fstream>
#include <future>
#include <iostream>
#include <set>
#include <sys/stat.h>

class RemoteMessage
{
private:
    std::future<void>             m_writeFuture;
    std::future<std::string>      m_readFuture;
    std::string                   m_id;
public:
    RemoteMessage(std::string id) : m_id(id) {}
    void exec()
    {
        m_writeFuture = std::async(std::launch::async, [this]
        {
            const char *pOutPath { "/tmp/pipeOut" };
            umask(0);
            mknod(pOutPath, S_IFIFO|0666, 0);
            std::ofstream ofs (pOutPath, std::ofstream::out);
            ofs << this->m_id << '\n';
        });
        m_readFuture = std::async(std::launch::async, [this]
        {
            std::string result;
            const char *pInPath { "/tmp/pipeIn" };
            umask(0);
            mknod(pInPath, S_IFIFO|0666, 0);
            std::ifstream ifs(pInPath);
            ifs >> result;
            return result;
        });
    }
    std::string getResult()
    {
        return m_readFuture.get();
    }
    bool resultIsReady() const
    {
        auto status = m_readFuture.wait_for(std::chrono::seconds(0));
        return (status == std::future_status::ready);
    }
};

void SendMessage(std::string pokemon)
{
    auto remoteM = RemoteMessage(pokemon);
    remoteM.exec();
    std::cout << remoteM.getResult() << "\n";
}

int main(int argc, const char * argv[]) {
    SendMessage("Bulbasaur");
    SendMessage("Squirtle");
    SendMessage("Charmander");
    SendMessage("Pickachu");
    SendMessage("Umbreon");
    return 0;
}

