def S():
    global cursor
    print(f"{cursor}    S -> AA")
    if A():
        if A():
            return True
        else:
            return False
    else:
        return False

def A():
    global cursor
    if cursor[0] == 'a':
        print(f"{cursor}    A -> aB")
        cursor = cursor[1:]
        if B():
            return True
        else:
            return False
    else:
        print(f"{cursor}    A -> ε")
        return True

def B():
    global cursor
    if cursor[0] == 'b':
        print(f"{cursor}    B -> b")
        cursor = cursor[1:]
        return True
    else:
        return False

string = input("Enter the string: ")
cursor = string
print("\nInput    Action")


if S() and cursor == "":
    print("String is successfully parsed")
else:
    print("Error in parsing String")