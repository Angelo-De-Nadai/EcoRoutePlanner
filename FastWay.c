/**
 * This C program models a system for managing a network of stations and cars. It defines structures for 'Station' and 'Car' nodes
 * and provides functions to perform various operations on them, including adding and deleting stations, adding and removing cars,
 * planning routes, and more. The program reads input from a file or standard input and processes a series of commands to manipulate
 * the station and car network. It uses a binary search tree to efficiently manage and search for stations based on their distances.
 *
 * The program includes functions to add, delete, and search for stations and cars, as well as functions for route planning and
 * car management. Additionally, it handles various scenarios, such as adding stations with associated cars, deleting stations with
 * and without cars, and planning routes based on user-defined criteria.
 *
 * Please note that this program assumes well-formed input and does not perform extensive error checking.
 */

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <stdint.h>

typedef struct Station {
    int distance;
    struct Car * carHead;
    struct Station * left;
    struct Station * right;
    int jumps;
    int previous; //referred to the array index in planRoute
}
Station;

typedef struct Car {
    int range;
    struct Car * next;
}
Car;

FILE * file;
int boolean = 0;
Station * root;

/**
 * Searches for a station with a specific distance.
 *
 * @param station The root station to start the search from.
 * @param number  The distance of the station to search for.
 * @return        Pointer to the found station, or NULL if not found.
 */
Station * searchStation(Station * station, int number) {
    if (station == NULL)
        return NULL;

    if (station -> distance == number)
        return station;
    if (station -> distance < number)
        return searchStation(station -> right, number);
    else
        return searchStation(station -> left, number);
}

/**
 * Removes a car with a specific range from a station.
 *
 * @param station The station from which to remove the car.
 * @param number  The range of the car to remove.
 */
void deleteCar(Station * station, int number) {
    if (station -> carHead == NULL)
        return;

    Car * current = station -> carHead;
    Car * previous = NULL;

    while (current != NULL && current -> range != number) { //search Car
        previous = current;
        current = current -> next;
    }

    if (current != NULL && previous == NULL) { //search for element in head 
        station -> carHead = station -> carHead -> next;
        free(current);
        boolean = 1;
        return;
    }

    if (current != NULL) { //search for element in middle or end 
        previous -> next = current -> next;
        free(current);
        boolean = 1;
    }

}

/**
 * Deletes a car from the system based on user input.
 *
 * @return "rottamata" if the car was successfully deleted, "non rottamata" otherwise.
 */
char * deleteCarSupport() {
    int num;
    int useless;
    fscanf(file, "%d", & num);

    //I look for the station and if it is NULL I stop reading the line of the file and exit
    Station * station = searchStation(root, num);
    if (station == NULL || station -> carHead == NULL) {
        fscanf(file, "%d", &useless);
        return "non rottamata";
    }

    //I read the numbver of the Car to be deleted and delete it
    fscanf(file, "%d", & num);
    boolean = 0;
    deleteCar(station, num);
    if (boolean == 0)
        return "non rottamata";
    else
        return "rottamata";
}

/**
 * Creates a new car with the given range.
 *
 * @param number  The range of the new car.
 * @return        Pointer to the newly created car.
 */
Car * buildCar(int number) {
    Car * nuovaAuto = (Car * ) malloc(sizeof(Car));
    nuovaAuto -> range = number;
    nuovaAuto -> next = NULL;
    return nuovaAuto;
}

/**
 * Adds a car to a station's list of cars, maintaining order.
 *
 * @param station The station to which the car should be added.
 * @param number  The range of the car to add.
 */
void addCar(Station * station, int number) {
    Car * nuovaAuto = buildCar(number);
    Car * current = station -> carHead;

    if (current == NULL || number >= current -> range) {
        nuovaAuto -> next = current;
        station -> carHead = nuovaAuto;
        return;
    }
    while (current -> next != NULL && current -> next -> range > number) {
        current = current -> next;
    }

    nuovaAuto -> next = current -> next;
    current -> next = nuovaAuto;
}


char * addCarSupport() {
    int num;
    fscanf(file, "%d", & num);
    Station * station = searchStation(root, num);

    //case the station does not exist
    if (station == NULL) {
        fscanf(file, "%d", & num);
        return "non aggiunta";
    }

    //add the station 
    fscanf(file, "%d", & num);
    addCar(station, num);
    return "aggiunta";
}

/**
 * Adds a new station to the system with a specific distance and associated cars.
 *
 * @return "aggiunta" if the station was successfully added, "non aggiunta" otherwise.
 */
Station * createStation(int number) {
    Station * newStation = (Station * ) malloc(sizeof(Station));
    newStation -> distance = number;
    newStation -> right = NULL;
    newStation -> left = NULL;
    newStation -> carHead = NULL;
    return newStation;
}

/**
 * Recursively adds a station to the binary search tree structure.
 *
 * @param current The current station being considered during the recursive process.
 * @param number  The distance of the station to add.
 * @return        Pointer to the updated station structure.
 */
Station * addStationRecursively(Station * current, int number) {
    if(current == NULL) {
        boolean = 1;
        return createStation(number);
    }

    if (current -> distance < number)
        current -> right = addStationRecursively(current -> right, number);
    else if (current -> distance > number)
        current -> left = addStationRecursively(current -> left, number);
    return current;
}

/**
 * Adds a new station to the system based on user input.
 *
 * @return "aggiunta" if the station was successfully added, "non aggiunta" otherwise.
 */
char * addStation() {
    int num;
    int num2;
    fscanf(file, "%d", & num);

    boolean = 0;
    //check if root exists
    if (root == NULL) {
        root = createStation(num);
        boolean = 1;
    } else {
        addStationRecursively(root, num);
    }

    //I empty the line if the station already exists
    if (boolean == 0) {
        char str[3000];
        fgets(str, 3000, file);
        return "non aggiunta";
    }

    Station * newStation = searchStation(root, num);
    fscanf(file, "%d", &num);

    //scroll through the cars to include in the station
    while (num != 0) {
        fscanf(file, "%d", & num2);
        num--;

        addCar(newStation, num2);
    }

    return "aggiunta";
}

/**
 * Removes all cars from a linked list of cars.
 *
 * @param current The head of the linked list of cars to be removed.
 */
void removeCarsList(Car * current) {
    Car * supp = current;
    while (supp != NULL) {
        supp = current -> next;
        free(current);
        current = supp;
    }
}

/**
 * Frees the memory of a station and its associated cars.
 *
 * @param station The station to be freed.
 */
void freeStation(Station * station) {
    removeCarsList(station -> carHead);
    station -> carHead = NULL;
    free(station);
}

/**
 * Searches for the minimum distance station in a binary search tree.
 *
 * @param current The current station being considered during the search.
 * @return        Pointer to the station with the minimum distance.
 */
Station * searchMinimum(Station * current) {
    while (current -> left != NULL) {
        current = current -> left;
    }
    return current;
}

/**
 * Deletes the station but not the cars from the binary search tree structure.
 *
 * @param current The current station being considered during the deletion process.
 * @param number  The distance of the station to delete.
 * @return        Pointer to the updated station structure.
 */
Station * deleteNoCarStation(Station * current, int number) {
    if (current == NULL)
        return current;

    if (current -> distance > number)
        current -> left = deleteNoCarStation(current -> left, number);
    else if (current -> distance < number)
        current -> right = deleteNoCarStation(current -> right, number);
    else {
        //no children
        if (current -> left == NULL && current -> right == NULL) {
            free(current);
            boolean = 1;
            return NULL;
        }
        //one child
        if (current -> left == NULL) {
            Station * supp = current -> right;
            free(current);
            boolean = 1;
            return supp;
        }
        if (current -> right == NULL) {
            Station * supp = current -> left;
            free(current);
            boolean = 1;
            return supp;
        }
        //case of two children
        boolean = 1;
        Station * supp = searchMinimum(current -> right);
        current -> distance = supp -> distance;
        current -> carHead = supp -> carHead;
        current -> right = deleteNoCarStation(current -> right, supp -> distance);
    }
    return current;
}

/**
 * Deletes a station from the binary search tree structure.
 *
 * @param current The current station being considered during the deletion process.
 * @param number  The distance of the station to delete.
 * @return        Pointer to the updated station structure.
 */
Station * deleteStation(Station * current, int number) {
    if (current == NULL)
        return current;

    if (current -> distance > number)
        current -> left = deleteStation(current -> left, number);
    else if (current -> distance < number)
        current -> right = deleteStation(current -> right, number);
    else {
        //no children
        if (current -> left == NULL && current -> right == NULL) {
            freeStation(current);
            boolean = 1;
            return NULL;
        }
        //one child
        if (current -> left == NULL) {
            Station * supp = current -> right;
            freeStation(current);
            boolean = 1;
            return supp;
        }
        if (current -> right == NULL) {
            Station * supp = current -> left;
            freeStation(current);
            boolean = 1;
            return supp;
        }
        //case of two children
        boolean = 1;
        removeCarsList(current -> carHead);
        Station * supp = searchMinimum(current -> right);
        current -> distance = supp -> distance;
        current -> carHead = supp -> carHead;
        current -> right = deleteNoCarStation(current -> right, supp -> distance);
    }
    return current;
}

/**
 * Deletes the root station from the binary search tree structure.
 *
 * @param number  The distance of the station to delete.
 */
void deleteRootStation(int number) {
    //no children
    if (root -> left == NULL && root -> right == NULL) {
        freeStation(root);
        root = NULL;
        return;
    }
    //one child
    if (root -> left == NULL) {
        Station * supp = root -> right;
        freeStation(root);
        root = supp;
        return;
    }
    if (root -> right == NULL) {
        Station * supp = root -> left;
        freeStation(root);
        root = supp;
        return;
    }
    //case of two children
    Station * supp = searchMinimum(root -> right);
    root -> distance = supp -> distance;
    removeCarsList(root -> carHead);
    root -> carHead = supp -> carHead;
    root -> right = deleteNoCarStation(root -> right, supp -> distance);
}

/**
 * Adds a new station to the system based on user input.
 *
 * @return "aggiunta" if the station was successfully added, "non aggiunta" otherwise.
 */
char * deleteStationSupport() {
    int num;
    fscanf(file, "%d", & num);

    //if no stations exist
    if (root == NULL)
        return "non demolita";

    //if the station to delete is the root
    if (root -> distance == num) {
        deleteRootStation(num);
        return "demolita";
    }

    //delete non-root station
    boolean = 0;
    deleteStation(root, num);
    if (boolean == 0)
        return "non demolita";
    else {
        return "demolita";
    }
}

/**
 * Searches for the next station with a greater distance from a given number.
 *
 * @param number  The distance of the current station.
 * @return        Pointer to the next station with greater distance.
 */
Station * nextStation(int number) {
    Station * best = NULL;
    Station * current = root;

    while (current != NULL) {
        if (current -> distance > number) {
            best = current;
            current = current -> left;
        } else {
            current = current -> right;
        }
    }
    return best;
}

/**
 * Searches for the previous station with a smaller distance from a given number.
 *
 * @param number  The distance of the current station.
 * @return        Pointer to the previous station with smaller distance.
 */
Station * previousStation(int number) {
    Station * best = NULL;
    Station * current = root;

    while (current != NULL) {
        if (current -> distance < number) {
            best = current;
            current = current -> right;
        } else {
            current = current -> left;
        }
    }
    return best;
}

/**
 * Creates an array of stations with infinity jumps for route planning.
 *
 * @param start  The distance of the starting station.
 * @param end    The distance of the ending station.
 * @return       An array of stations with infinity jumps.
 */
Station ** createPathArray(int start, int end) {
    Station * first = searchStation(root, start);
    Station * station = first;
    int counter = 1;
    while (station -> distance != end) {
        counter++;
        station = nextStation(station -> distance);
    }
    boolean = counter;

    Station ** path = (Station ** ) malloc(sizeof(Station * ) * counter);

    station = first;
    for (int i = 0; i < counter; i++) {
        path[i] = station;
        path[i] -> previous = -1;
        path[i] -> jumps = 0;
        station = nextStation(station -> distance);
    }
    path[0] -> previous = 0;

    return path;
}

/**
 * Creates an array of stations with infinity jumps for route planning.
 *
 * @param start  The distance of the starting station.
 * @param end    The distance of the ending station.
 * @return       An array of stations with infinity jumps.
 */
Station ** createPathArray2(int start, int end) { //I set the jumps to infinity
    Station * first = searchStation(root, start);
    Station * station = first;
    int counter = 1;
    while (station -> distance != end) {
        counter++;
        station = nextStation(station -> distance);
    }
    boolean = counter;
    Station ** path = (Station ** ) malloc(sizeof(Station * ) * counter);

    station = first;
    for (int i = 0; i < counter; i++) {
        path[i] = station;
        path[i] -> previous = -1;
        path[i] -> jumps = INT32_MAX;
        station = nextStation(station -> distance);
    }
    path[0] -> previous = 0;

    return path;
}

/**
 * Plans a direct route from start station to end station.
 *
 * @param start  The distance of the starting station.
 * @param end    The distance of the ending station.
 */
void directPlanRoute(int start, int end) {

    //case start and end stations coincide
    if (start == end) {
        if (searchStation(root, start) == NULL) {
            printf("nessun percorso\n");
            return;
        } else {
            printf("%d\n", start);
            return;
        }
    }

    Station ** path = createPathArray(start, end);
    int nElement = boolean;
    boolean = 0;
    int j = 1; //useful for resuming from the last item written

    for (int i = 0; i < nElement; i++) {
        if (path[i] -> previous == -1) {
            break;
        }
        //if it can actually proceed
        if (path[i] -> carHead != NULL) {
            int distMax = path[i] -> distance + path[i] -> carHead -> range;

            //iterate all items in front of first and update the status
            while (j < nElement && path[j] -> distance <= distMax) {
                path[j] -> jumps = path[i] -> jumps + 1;
                path[j] -> previous = i;
                if (path[j] -> distance == end) {
                    boolean = 1;
                    break;
                }
                j++;
            }
            if (boolean == 1)
                break;
        }
    }

    //press preparation  
    int jumps = path[nElement - 1] -> jumps + 1;
    if (jumps == 1) {
        printf("nessun percorso\n");
        return;
    }

    //print order reversal
    int numberStation[jumps];
    j = nElement - 1;
    for (int i = jumps - 1; i != -1; i--) {
        numberStation[i] = path[j] -> distance;
        j = path[j] -> previous;
    }
    //actual print
    for (int i = 0; i < jumps - 1; i++) {
        printf("%d ", numberStation[i]);
    }
    printf("%d\n", numberStation[jumps - 1]);

}

/**
 * Plans an inverse route from the higher station to the lower station.
 *
 * @param alto   The distance of the higher station.
 * @param basso  The distance of the lower station.
 */
void inversePlanRoute(int alto, int basso) {

    //initialize first stations
    Station ** path = createPathArray2(basso, alto);
    int nElement = boolean;
    int i = nElement - 1;
    int j = i;
    boolean = 0;
    path[i] -> jumps = 0;
    if (path[i] -> carHead == NULL) {
        printf("nessun percorso\n");
        return;
    }

    //I initialize support stations
    int farthest = i;
    int block = i;
    int largestDisplacement = i;

    while (i != 0) {
        do {
            if (path[i] -> carHead != NULL) {
                int distMax = path[i] -> distance - path[i] -> carHead -> range;
                //if distMax exceeds the end I take the element in position 0, update it and exit
                if (distMax <= basso) {
                    path[0] -> previous = i;
                    path[0] -> jumps = path[i] -> jumps + 1;
                    boolean = 1;
                    break;
                }

                //I go to the furthest position possible with second
                while (path[j] -> distance >= distMax) {
                    j--;
                }
                j++;

                if (j < farthest) { //I move the Farthest pointer to the furthest position
                    farthest = j;
                }

                while (j != i) {
                    //I check if the current (second) station can be reached with fewer hops
                    if (path[j] -> jumps > path[i] -> jumps + 1) {
                        path[j] -> jumps = path[i] -> jumps + 1;
                        path[j] -> previous = i;
                    }
                    j++;
                }

            }
            if (i != block) {
                i++;
            }
        } while (i != block);

        if (boolean == 1) // in case the end has been reached
            break;

        if (farthest == largestDisplacement) { //case in which it is no longer possible to proceed due to the absence of machine jumps
            printf("nessun percorso\n");
            return;
        }

        //recalibrate pointers
        i = farthest;
        block = largestDisplacement;
        largestDisplacement = i;
    }

    //press preparation
    int jumps = path[0] -> jumps + 1;
    if (jumps == 0) {
        printf("nessun percorso\n");
        return;
    }

    //print order reversal
    int numberStation[jumps];
    j = 0;
    for (i = jumps - 1; i != -1; i--) {
        numberStation[i] = path[j] -> distance;
        j = path[j] -> previous;
    }
    //actual print
    for (i = 0; i < jumps - 1; i++) {
        printf("%d ", numberStation[i]);
    }
    printf("%d\n", numberStation[jumps - 1]);
}

/**
 * Plans a route based on user input, considering both direct and inverse routes.
 */
void planRoute() {
    int num;
    int num2;
    fscanf(file, "%d %d", & num, & num2);

    if (num <= num2) {
        directPlanRoute(num, num2);
    } else {
        inversePlanRoute(num, num2);
    }
}

int main(int argc, char * argv[]) {
    file = stdin;
    char str[30];
    fscanf(file, "%s", str);

    do {
        if (!strcmp(str, "aggiungi-stazione")) {
            printf("%s\n", addStation());
        } else if (!strcmp(str, "demolisci-stazione")) {
            printf("%s\n", deleteStationSupport());
        } else if (!strcmp(str, "aggiungi-auto")) {
            printf("%s\n", addCarSupport());
        } else if (!strcmp(str, "rottama-auto")) {
            printf("%s\n", deleteCarSupport());
        } else if (!strcmp(str, "pianifica-percorso")) {
            planRoute();
        }
        fscanf(file, "%s", str);

    } while (feof(file) == 0);
}