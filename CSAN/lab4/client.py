from ftplib import FTP

def printMenu():
    print("Select the command:")
    print("1. List files in current directory")
    print("2. Change current directory")
    print("3. Remove file")
    print("4. Make directory")
    print("5. Remove directory")
    print("6. Download file")
    print("7. Upload file")
    print("0. Exit")

ftp = FTP()
print("Welcome to the FTP Client!")
print("Enter the domain name:")
domain = input()
print("Enter the port number:")
port = input()
try:
    if (len(port) != 0):
        ftp.connect(domain, int(port))
    else:
        ftp.connect(domain)
except:
    print("An error occured connecting to host.")
    exit()
print("Connected...")
print(ftp.getwelcome())
ftp.login()
notFinished = True

printMenu()

while (notFinished):
    command = input()
    if command == "1":
        ftp.retrlines("LIST")
    elif command == "2":
        path = input("Enter the path to change to:\n")
        try:
            ftp.cwd(path)
            print("Path changed to {}".format(path))
        except:
            print("An error occured changing to entered directory.")
    elif command == "3":
        name = input("Enter the name of a file to remove:\n")
        try:
            ftp.delete(name)
            print("File '{}' was successfully deleted".format(name))
        except:
            print("An error occured deleting the file")
    elif command == "4":
        name = input("Enter directory name:\n")
        try:
            ftp.mkd(name)
            print("Directory '{}' was successfully created".format(name))
        except:
            print("An error occured creating directory")
    elif command == "5":
        name = input("Enter directory name to remove:\n")
        try:
            ftp.rmd(name)
            print("Directory '{}' was successfully removed".format(name))
        except:
            print("An error occured removing directory")
    elif command == "6":
        name = input("Enter the name of a file to download:\n")
        try:
            ftp.retrbinary('RETR ' + name, open(name, 'wb').write)
            print("File '{}' is downloaded successfully".format(name))
        except:
            print("An error occured downloading file")
    elif command == "7":
        name = input("Enter the name of a file to upload:\n")
        try:
            ftp.storbinary("STOR " + name, open(name, 'rb'))
            print("File '{}' is successfully uploaded".format(name))
        except:
            print("An error occured uploading file")
    elif command == "0":
        notFinished = False
    else:
        printMenu()
