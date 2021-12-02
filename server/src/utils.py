import re

def publicate(file_path):
    file = open(file_path, "r")
    text = file.read()
    file.close()

    text = re.sub("private", "public", text)

    file = open(file_path, "w")
    file.write(text)
    file.close()