#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "BmrAndBmi.h" //header file!

// Patrick Leimer 4/10/2023 

// structs
typedef struct
{
    float age;
    float weight;
    float height;
    char gender;

} PERSON;

typedef struct
{
    int meals;
    int ingredients;
    float foodWeight;
    char ingredient[15];
} MEALS;

typedef struct
{
    char weightGoal;
    float bmi;
    float bmr;
    time_t date;
    float eatenCal;
    char failOrPassbmr;
    PERSON person;
    int recordsInArr;
} RECORD;

typedef struct
{
    char food[15];
    int calPerPound;
    int foodsInArr;
} FOOD;

void menuPrint(char *choice);
int fitnessTracker(RECORD *person);
int dailyEatCheckIn(RECORD *person, FOOD *plates, MEALS *personMeals);
int fitnessTrackerPrinter(RECORD *person, char *choice);
int foodDatabasePrint(FOOD *plates);

int main()
{
    char choice = ' ';
    RECORD *person = (RECORD *)malloc(150 * sizeof(RECORD)); // disjointed memory arrays
    FOOD *plates = (FOOD *)malloc(200 * sizeof(FOOD));
    MEALS *personMeals = (MEALS *)malloc(sizeof(MEALS));
    if (person == NULL || plates == NULL || personMeals == NULL)
    {
        return 1;
    }

    // MENU
    while (choice != 'Q')
    {
        menuPrint(&choice);

        switch (choice)
        {

        case 'A': // enter information to fitness tracker.
            fitnessTrackerPrinter(person, &choice);
            dailyEatCheckIn(person, plates, personMeals);
            fitnessTracker(person);
            break;

        case 'B': // print fitness progress throughout time
            fitnessTrackerPrinter(person, &choice);
            break;

        case 'C':
            foodDatabasePrint(plates);
            break;

        case 'Q':
            break;

        default:
            printf("Invalid choice, try again!");
            break;
        } // end switch

    } // end while

    free(person);
    free(plates);
    free(personMeals);

    return 0;
}

void menuPrint(char *choice)
{
    printf("[A] Add New Information To Fitness Tracker.\n[B] Print Fitness tracker page.\n[C] Print Food Database.\n[Q] Quit.\nChoice: ");
    scanf(" %c", choice);
    *choice = toupper(*choice); // makes upper case
    getchar(); // Consume any leftover characters, especially newlines

}

int fitnessTracker(RECORD *person)
{

    int pos = person->recordsInArr;

    // GET TIME
    time(&(person + pos)->date);

    // GET GENDER
    if (person->person.gender != 'M' && person->person.gender != 'F')
    {
        while (person->person.gender != 'M' && person->person.gender != 'F')
        {
            printf("Please enter your gender. [M] Male [F] Female\n");
            scanf(" %c", &person->person.gender);
            person->person.gender = toupper(person->person.gender);
        }
    }

    // GET WEIGHT
    float *weight = (float *)malloc(sizeof(float));
    printf("What is your current weight? Please input in kilograms\n");
    scanf("%f", weight);
    getchar();

    while (*weight < 10 || *weight > 999)
    {
        printf("Invalid input, try again.\n");
        scanf("%f", weight);
        getchar();
    }
    (person + pos)->person.weight = *weight;

    // GET HEIGHT
    float *height = (float *)malloc(sizeof(float));
    printf("What is your height? Please input in centimeters\n");
    scanf("%f", height);
    getchar();

    while (*height < 0 || *height > 250)
    {
        printf("Invalid input, try again.\n");
        scanf("%f", height);
        getchar();
    }
    (person + pos)->person.height = *height;

    // GET AGE
    int *age = (int *)malloc(sizeof(int));
    printf("Enter your age.\n");
    scanf("%i", age);
    getchar();

    while (*age < 0 || *age > 150)
    {
        printf("Invalid input, try again.\n");
        scanf("%i", age);
        getchar();
    }
    (person + pos)->person.age = *age;

    // WEIGHT GOAL
    while ((person + pos)->weightGoal != 'G' && (person + pos)->weightGoal != 'S' && (person + pos)->weightGoal != 'L')
    {
        printf("What is your current weight goal? [G] Gain [S] Stay [L] Lose\n");
        scanf(" %c", &(person + pos)->weightGoal);
        (person + pos)->weightGoal = toupper((person + pos)->weightGoal);
    }

    // BMR & BMI
    printf("\nAs calculated before, you consumed a total of %.0f calories today\n", (person + pos)->eatenCal);
    float bmiandbmr;
    getBMI(*weight, *height, &bmiandbmr); // both
    (person + pos)->bmi = bmiandbmr;
    printf("Your current BMI is %.1f\n", (person + pos)->bmi);

    if (person->person.gender == 'F')
    {
        getWBMR(*weight, *height, *age, &bmiandbmr); // females
        (person + pos)->bmr = bmiandbmr;
    }
    else
    {
        getMBMR(*weight, *height, *age, &bmiandbmr); // males
        (person + pos)->bmr = bmiandbmr;
    }
    printf("Given that your current BMR is of %.0f calories, the difference between your calories consumed today and your BMR: %.2f \n", (person + pos)->bmr, ((person + pos)->eatenCal - (person + pos)->bmr));

    if (((person + pos)->eatenCal < (person + pos)->bmr) && (person + pos)->weightGoal == 'L')
    { // success if trying to loose weight
        printf("You were successful at eating less calories than your body burns, keep it up and loose weight!\n");
        (person + pos)->failOrPassbmr = 'P';
    }
    else if (((person + pos)->eatenCal > (person + pos)->bmr) && (person + pos)->weightGoal == 'G')
    { // success if trying to gain weight
        printf("You were successful at eating more calories than your body burns, keep it up and gain weight!\n");
        (person + pos)->failOrPassbmr = 'P';
    }
    else if (((person + pos)->eatenCal < ((person + pos)->bmr + 100)) && (person + pos)->weightGoal == 'S')
    { // success if trying to stay at weight
        printf("You were successful at eating needed calories to stay at weight, keep it up!\n");
        (person + pos)->failOrPassbmr = 'P';
    }
    else
    {
        printf("You were not successful at meeting your weight goal, keep track of those calories consumed and you got it!\n");
        (person + pos)->failOrPassbmr = 'F';
    }

    (person->recordsInArr)++;

    printf("\n%.2f\t%c\t%.1f\t%.0f\t%.0f\t%c\t%s\n", (person + pos)->person.weight, (person + pos)->weightGoal, (person + pos)->bmi, (person + pos)->eatenCal, (person + pos)->bmr, (person + pos)->failOrPassbmr, ctime(&(person + pos)->date));

    free(weight);
    free(height);
    free(age);

    FILE *fitTrackBin1 = fopen("/usr/src/app/data/fitnessTracker.bin", "wb");
    if (fitTrackBin1 == NULL)
    {
        return 1;
    }
    fwrite(person, sizeof(RECORD), 150, fitTrackBin1);
    fclose(fitTrackBin1);

    return 0; // Success

} // end function

int dailyEatCheckIn(RECORD *person, FOOD *plates, MEALS *personMeals)
{
    int i, j, m;
    FILE *foodsBin = fopen("/usr/src/app/data/foodDataBase.bin", "rb"); // receive input from bin file, fixed for docker
    if (foodsBin == NULL)
    {
        return 1;
    }

    fread(plates, sizeof(FOOD), 200, foodsBin);
    fclose(foodsBin);

    int y = person->recordsInArr;
    (person + (y))->eatenCal = 0;

    printf("How many meals did you have today?\n");
    scanf("%i", &personMeals->meals); // meals amount

    for (i = 0; i < personMeals->meals; i++)
    {
        printf("How many ingredients did your meal %i have?\n", i + 1);
        scanf("%i", &personMeals->ingredients); // amount of ingredients in meal i
        getchar(); // Clear the newline left in the buffer after reading an integer

        for (j = 0; j < personMeals->ingredients; j++)
        {
            bool foundInDataBase = false;

            printf("Enter ingredient %i name: \n", j + 1);
            fflush(stdin);
            scanf("%15[^\n]%*c", personMeals->ingredient);
            sentenceCase(personMeals->ingredient); // sentence case for the string compare to be accurate

            printf("Enter the amount in pounds you ate of ingredient %i:\n", j + 1);
            scanf("%f", &personMeals->foodWeight);
            getchar();

            int k = plates->foodsInArr;

            for (m = 0; m < k; m++)
            { // here is the searching in array
                if (strcmp(personMeals->ingredient, (plates + m)->food) == 0)
                {                                                                                      // found in database //fix the space problem??
                    (person + (y))->eatenCal += ((plates + m)->calPerPound * personMeals->foodWeight); // fix
                    foundInDataBase = true;
                }

                if (m + 1 == k && foundInDataBase == false)
                { // if the food is not in the database
                    printf("There is no record of this ingredient, please enter name again.\nName: ");
                    scanf("%15[^\n]%*c", (plates + (k))->food);
                    sentenceCase((plates + (k))->food); // sentence case for it to save in the database in correct format

                    printf("Please enter the ingredient's calories per pound\nCal Per Pound: ");
                    scanf("%i", &(plates + (k))->calPerPound);
                    getchar();

                    (person + (y))->eatenCal += ((plates + (k))->calPerPound * personMeals->foodWeight); // fix
                    (plates + (k))->foodsInArr = 0;
                    (plates->foodsInArr)++;

                    foundInDataBase = true;
                }

            } // end for m

        } // end for j

    } // end for i

    // SORT
    int pass, temp;
    char *temp2 = (char *)malloc(15 * sizeof(char));

    for (pass = 1; pass < plates->foodsInArr; pass++)
    { // here is the sorting
        for (i = 0; i < plates->foodsInArr; i++)
        {

            if ((plates + i)->calPerPound < (plates + (i + 1))->calPerPound)
            {
                temp = (plates + i)->calPerPound; // cal
                (plates + i)->calPerPound = (plates + (i + 1))->calPerPound;
                (plates + (i + 1))->calPerPound = temp;
                // food name swap
                strcpy(temp2, (plates + i)->food);
                strcpy((plates + i)->food, (plates + (i + 1))->food);
                strcpy((plates + (i + 1))->food, temp2);
            }
        }
    }

    printf("You consumed a total of %.0f calories today\n\n", (person + (y))->eatenCal);

    FILE *addFoodBin = fopen("/usr/src/app/data/foodDataBase.bin", "wb"); // save information into bin file
    if (foodsBin == NULL)
    {
        return 1;
    }

    fwrite(plates, sizeof(FOOD), 200, foodsBin);

    fclose(foodsBin);

    return 0; // Success

} // end function

int fitnessTrackerPrinter(RECORD *person, char *choice)
{

    FILE *fitTrackBin = fopen("/usr/src/app/data/fitnessTracker.bin", "rb"); //fixed for docker
    if (fitTrackBin == NULL)
    {
        return 1;
    }
    fread(person, sizeof(RECORD), 150, fitTrackBin);
    fclose(fitTrackBin);

    if (*choice != 'A')
    {
        int i, pos = person->recordsInArr;
        printf("\nWeight | Weight Goal | BMI | Eaten Calories | BMR | Met goal? | Date | \n\n");
        for (int i = 0; i < pos; i++)
        {
            printf("%.2f | %c | %.1f | %.0f | %.0f | %c | %s ", (person + i)->person.weight, (person + i)->weightGoal, (person + i)->bmi, (person + i)->eatenCal, (person + i)->bmr, (person + i)->failOrPassbmr, ctime(&(person + i)->date));
        }
        printf("\n");
    }
    return 0; // Success
}

int foodDatabasePrint(FOOD *plates)
{
    FILE *foodsBin = fopen("/usr/src/app/data/foodDataBase.bin", "rb"); // receive input from bin file, fixed for docker
    if (foodsBin == NULL)
    {
        return 1;
    }

    fread(plates, sizeof(FOOD), 200, foodsBin);

    fclose(foodsBin); //closes file
    int i;
    printf("Ingredients Database: \n\n");
    printf("Ingredient | Calories\n");
    for (i = 0; i < plates->foodsInArr; i++)
    {
        printf("%s | %i\n", (plates + i)->food, (plates + i)->calPerPound);
    }
    printf("\n");
    return 0; // Success
}

/*
person struct = age, weight in kg, gender, height in cm, bmi, bmr

database of foods:
    Stored in a bin file
    struct of food, calorie per pound, foods in array
-------------------------------------------------------

FUNCTION: FITNESS TRACKER GET INFORMATION
get weight, date, calories eaten that they, BMI, BMR, weight goal, daily success or fail for weight goal based on BMR
person struct = age, weight in kg, gender, height in cm,
  ----------------------------------------------------------
 BMR, tell if user is over or under BMR, tell if it failed or passed
 //header file
getMBMR(w, h, a, &bmr ); //mens
getWBMR(w, h, a, &bmr ); //females
getBMI(w, h, &bmi); //both

if ( totalCaloriesThisDay < bmr ) //success if trying to loose weight
if ( totalCaloriesThisDay > bmr ) //success if trying to gain weight
if ( totalCaloriesThisDay < bmr + 100 )  //success if trying to stay at weight
printf("Your BMR is %.0f calories, and your BMI is %.1f kg/msqr\n", bmr, bmi);

--------------------------------------------------------------------------------------
FUNCTION: DAILY CHECK IN
Enter food, calorie calculator for the day ////--------------------------------

ask user how many meals they had today
  loop i < amount of meals {
  print( how many ingredients did your meal have?)
      loop j < amount of ingredients {
        print ( Enter 1,2... ingredient )
        fgets
        print ( Enter 1,2... ingredient amount had in pounds)
        scanf %s

       for (m = 0; m < foods in array; m++) {

         if ( strcmp( ingridient entered, *(food + m).food  ) == 0 ) {
        totalCaloriesThisDay +=  ( *(food + m).foodCal * amount had in pounds )
       }

       if( m+1 == foods in array) {

        printf( There is no record of this ingredient, please enter name)
        fgets
        printf ( what is the foods calories per pound)
        scanf %i

    SORT foods
    add to the foods in array struct

       }

     }
}
  }


FUNCTION: FITNESS TRACKER PRINT PROGRESS
FUNCTION:  PRINT FOOD DATA BASE



*/

/*
float w = 62, h = 187, a = 19, bmr, bmi;

getMBMR(w, h, a, &bmr );

getBMI(w, h, &bmi);

printf("Your BMR is %.0f calories, and your BMI is %.1f kg/msqr\n", bmr, bmi);
*/
