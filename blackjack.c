#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

int deck[52]; //deck of cards
int shuffDeck[52]; //temporary array to shuffle cards
int cash = 1000; //money user has
int bet = 0; // value of bet
int myh[10]; // arrangement of cards in my hand
int dh[10]; // arrangement of card in dealer's hand
int dh_sum = 0; // sum of dealer's hand
int myh_sum = 0; // sum of my hand
char resp[5]; // user response
int betTemp = 0; // how much the user would like to bet 
bool equal = true; //checks for equality of the two hands
bool iWin(); // checks if users hand meets conditions to win
void stay(); // consolidates iWin check into a method
int myhc = 0; // my hand counter
int z = 0; //counter

void play();
void shuffle();
int add(int elements, int array[]);
void option(); // hit or stay
void aCdh(); //aise check dealer's hand
void aCmyh(); //aise check my hand
void myhIS();

int main() //no command line arguments required
{
    int deckC = 0; //counter for element of deck array
    
    for(int i = 2; i < 10; i++) //assigns values to the deck array for digit cards
    {
        for(int j = 0; j < 4; j++)
        {
            deck[deckC] = i;
            deckC++;
        }
    }
    
    for(int i = 0; i < 16; i++) //assigns values to the deck array for face cards and tens
    {
        deck[deckC] = 10;
        deckC++;
    }
    
    for(int i = 0; i < 4; i++) //assigns values to the deck array for aise cards
    {
        deck[deckC] = 11;
        deckC++;
    }
    
    srand(time(NULL)); // allows me to produce random numbers
    
    string rules = "Welcome to BlackJack! Please read these rules before playing: \nAll faces cards are worth 10 while aises are worth 1 or 11(whichever makes the better hand)\nYour main objective is to beat the dealer's hand without going over 21.\nType 'hit' to ask for another card and type 'stay' to stay with your current hand.\nIf you need to access these rules during the game just type 'help' at any time.\nYou will start each game with $1000 so bet wisely and enjoy the game!\n";
    printf("%s\n", rules);

    char input[6]; //keeps track of user input
    printf("Type 'deal' to start the game and to continue playing(type 'exit' to leave the game).\n");

    scanf("%s", input);
    
    while(strcasecmp(input, "exit") != 0)
    {
        if(strcasecmp(input, "deal") == 0)
        {
            if(bet > -1 && cash >= bet)
            {
                for(int i = 0; i < 10; i++)
                {
                    myh[i] = 0;
                    dh[i] = 0;
                }
                shuffle();
                play();
            }
            else
            {
                printf("You're out of money :(. Game Over\n");
            }
        }
        else if(strcasecmp(input, "help") == 0)
        {
            printf("%s\n", rules);
        }
        else
        {
            printf("Please enter a valid input.\n");
        }
        scanf("%s", input);
    }
    return 0;
}

void play()
{
    dh_sum = 0;
    myh_sum = 0;
    z = 0;
    myhc = 0;
    printf("You have $%i\nThe current bet is $%i\n", cash, bet); // shows user the amount of money that they are working with
    
    
    printf("Would you like to raise or stay?\n");
    scanf("%s", resp); // accepts users choice to bet
    
    while(strcasecmp(resp, "raise") != 0 && strcasecmp(resp, "stay") != 0)
    {
        printf("Please enter raise or stay.\n");
        scanf("%s", resp);
    }
    
    if(strcasecmp(resp, "raise") == 0)
    {
        printf("How much would you like to bet?\n"); // game begins with increased bet
        betTemp = get_int("");
        bet += betTemp;
        printf("The bet is now $%i\n", bet);
        
        if(cash - bet < 0)
        {
            while(cash - bet < 0)
            {
                bet -= betTemp;
                printf("Bet a smaller amount\n"); // corrects for invalid input
                betTemp = get_int("");
                bet += betTemp;
                printf("The bet is now $%i\n", bet);
            }
        }
    }
    
    else if(strcasecmp(resp, "stay") == 0) // game begins with same bet as before
    {
        printf("Let the games begin.\nThe bet is still $%i\n", bet);
    }
    
    cash -= bet; // temporarily subtracts bet from cash
    
    equal = dh_sum == myh_sum; // checks for equality of the two hands
    
    while(dh_sum < 18)
    {
        dh[z] = deck[z];
        z++;
        dh_sum = add(z, dh);
        aCdh(z);
        dh_sum = add(z, dh);
    }
    
    dh_sum = add(z, dh);
    
    z++;
    myh[0] = deck[z];
    myhc++;
    z++;
    myh[1] = deck[z];
    myhc++;
    myh_sum = add(2, myh);
    
    if((myh_sum == 21) && (dh[0] + dh[1]) == 21) // if we both get perfect blackjack
    {
        printf("You and the dealer have equal hands, so nobody wins.\n");
        cash += bet;
    }
    
    else if(myh_sum == 21)
    {
        printf("You win! You had a perfect blackjack!\n");
        cash += (2 * bet);
    }
    
    else if(dh[0] + dh[1] == 21)
    {
        printf("You lose. The dealer had a perfect blackjack.\n");
    }
    else
    {
        option();
    }

    // add in the adjustment for aises and add output as to how the computer will tell the user their hand while simultaneously keeping
    //track ot he dealer's hand
}

void shuffle()
{
    int r = rand() % 52;
    for(int i = 0; i < 52; i++)
    {
        if(deck[r] != 0)
        {
            shuffDeck[i]= deck[r];
            deck[r] = 0; //temporary value
        }
        else
        {
            while(deck[r] == 0)
            {
                r = rand() % 52;
            }
            shuffDeck[i]= deck[r];
            deck[r] = 0; //temporary value
        }
    }
    for(int i = 0; i < 52; i++) // reassigns values from shuffDeck[] to deck[]
    {
        deck[i] = shuffDeck[i];
    }
}

int add(int elements, int array[])
{
    int sum = 0;
    for(int i = 0; i < elements; i++)
    {
        sum += array[i];
    }
    return sum;
}

bool iWin()
{
    if(myh_sum > dh_sum && myh_sum <= 21)
    {
        return true;
    }
    else if(myh_sum < dh_sum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool dWin()
{
    if(dh_sum > myh_sum && dh_sum <= 21)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void stay()
{
    if(dh_sum == myh_sum)
    {
        printf("You and the dealer have equal hands, so nobody wins.\n");
        cash += bet;
    }
    else if(iWin())
    {
        printf("You win!\n");
        cash += (2 * bet);
    }
    else
    {
        printf("You lose.\n");
    }
}

void option()
{
        myhIS();
        char dec[5]; // stores user's decision to hit or stay(dealer's decision will be automated)
        printf("Hit or stay?\n");
        scanf("%s", dec);
        
        while(strcasecmp(dec, "hit") != 0 && strcasecmp(dec, "stay") != 0)
        {
            printf("Please enter hit or stay.\n");
            scanf("%s", dec);
        }
        
        if(strcmp(dec, "hit") == 0)
        {
            z++;
            myhc++;
            myh[myhc - 1] = deck[z];
            myh_sum = add(myhc, myh);
            aCmyh(myhc);
            myh_sum = add(myhc, myh);
            

            if(myh_sum > 21)
            {
                myhIS();
                if(dh_sum == myh_sum) //checks for equality of hands
                {
                    printf("You and the dealer have equal hands, so nobody wins.\n");
                    cash += bet;
                }
                else
                {
                    stay();
                }
            }
            else
            {
                option();
            }
        }
        else
        {
            myhIS();
            stay();
        }
}

void aCdh(int length) //aise check for dh
{
    if(dh_sum > 21)
    {
        for(int i = 0; i < length; i++)
        {
            if(dh[i] == 11)
            {
                dh[i] = 1;
            }
        }
    }
}

void aCmyh(int length) // aise check for myh
{
    if(myh_sum > 21)
    {
        for(int i = 0; i < length; i++)
        {
            if(myh[i] == 11)
            {
                myh[i] = 1;
            }
        }
    }
}

void myhIS()
{
    printf("Your hand is a %i and a %i ", myh[0], myh[1]); 
    for(int i = 2; i < myhc; i++)
    {
        printf("and a %i ", myh[i]);
    }
    printf("\n");
}
