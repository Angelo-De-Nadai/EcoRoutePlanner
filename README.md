# EcoRoutePlanner
## Introduction

This C program models a system for managing a network of stations and cars. It defines structures for 'Station' and 'Car' nodes
and provides functions to perform various operations on them, including adding and deleting stations, adding and removing cars,
planning routes, and more. The program reads input from a file or standard input and processes a series of commands to manipulate
the station and car network. It uses a binary search tree to efficiently manage and search for stations based on their distances.

The program includes functions to add, delete, and search for stations and cars, as well as functions for route planning and
car management. Additionally, it handles various scenarios, such as adding stations with associated cars, deleting stations with
and without cars, and planning routes based on user-defined criteria.

## Project Description

The project focuses on modeling a highway as a sequence of service stations, each uniquely identified by its distance from the beginning of the highway, expressed in kilometers as a positive or zero integer.

Each service station is equipped with a fleet of electric rental vehicles, each characterized by a range expressed in kilometers as a positive integer. The vehicle fleet of a single station can include up to 512 vehicles.

A journey is defined by a sequence of service stations, starting at one station and ending at another, passing through zero or more intermediate stations. During the journey, the driver cannot backtrack and rents a new vehicle each time they stop at a station.

The goal of the project is to plan a route between two specified stations with the minimum number of stops. In case of multiple routes with the same minimum number of stops, the project should select the route that prefers stops closer to the beginning of the highway.

## Example of Highway

Consider an example of a highway with stations and vehicle autonomy:

```
Start   20   30   45   50
Vehicle autonomy: 5, 10, 15, 25
```

In this example, the correct route between the station at distance 20 and the one at distance 50 is 20 → 30 → 50 (not 20 → 45 → 50). Conversely, 50 → 30 → 20 is the correct route between the station at distance 50 and the one at distance 20 (from right to left).

## Input File Format and Expected Output

The textual input file contains a sequence of commands, each with the following format:

- `add-station distance vehicle-number vehicle-autonomy-1 ... vehicle-autonomy-n`: Adds a station at the specified distance with a specified fleet of vehicles. Prints "added" or "not added" in response.

- `demolish-station distance`: Removes the station at the specified distance if it exists. Prints "demolished" or "not demolished" in response.

- `add-vehicle station-distance vehicle-autonomy-to-add`: Adds a vehicle to the station if it exists. Prints "added" or "not added" in response.

- `scrap-vehicle station-distance vehicle-autonomy-to-scrap`: Removes a vehicle from the station if it exists and has at least one vehicle with the specified autonomy. Prints "scrapped" or "not scrapped" in response.

- `plan-route start-station-distance end-station-distance`: Plans the route between the two specified stations. Prints the stops in order of traversal, separated by spaces, followed by a newline. Departure and arrival must be included; if they coincide, the station is printed only once. If the route does not exist, prints "no route."

The planning action does not alter the stations or their vehicle fleets, and the given stations are definitely present.

## Example Usage

### Input
```
add-station 10 3 100 200 300
add-station 20 2 50 100
plan-route 10 20
```

### Output
```
20 30
```

In this example, two stations are added, and then a route is planned between the station at distance 10 and the one at distance 20. The optimal route is through stops 20 and 30.
