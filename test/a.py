import b

test = 1

def change():
    global test
    test +=  1
    return test


for _ in range(5):
    print(change())