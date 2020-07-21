def GrassMethod(id):
    return id+":GrassType'\n"

def WaterMethod(id):
    return id+":WaterType\n"

def FireMethod(id):
    return id+":FireType\n"

def ElectricMethod(id):
    return id+":ElectricType\n"

def UnknownMethond(id):
    return id+":UnknownType"

messageResponses = {
    "Bulbasaur" : GrassMethod,
    "Squirtle" : WaterMethod,
    "Charmander" : FireMethod,
    "Pickachu" : ElectricMethod,
}

while True:
    content = ""
    with open("/tmp/pipeOut", "r") as f:
        content = f.readline().strip()
        if len(content) > 0:
            print(content)
            pIn = open("/tmp/pipeIn", "w")
            if content in messageResponses:
                pIn.write(messageResponses[content](content))
            else:
                pIn.write(UnknownMethond(content))
            pIn.flush()
            pIn.close()
