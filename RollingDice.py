import random
def generateRandomNumber(maximumLimit):
    randomNumber=random.randint(1, maxLimit)
    return randomNumber


def main():
    maximumLimit=6
    isDiceRoll=True
    while isDiceRoll:
        userInput=input("Ready to roll? Enter Q to Quit")
        if userInput.lower() !="q":
            randomNumber = fun(maximumLimit)
            print("You have rolled a",randomNumber)
        else:
            isDiceRoll=False