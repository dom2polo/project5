Group Project 5

This internet type sockets program consist of two distributed program that implements two mutual exclusion algorithm. The centralized and token ring algorithms will be used to solve the Dining Philosophers Problem. 

Defn: The Dining Philosophers problem is a classic computer science problem that illustrates the challenges of managing shared resources in a distributed system. It involves a group of philosophers sitting at a table with chopsticks, and trying to eat without causing a deadlock or starvation 

Centralized Algorithm: 
  - relies on a central authority to coordinate and manage the actions of a distributed system. 
  - single entity that controls access to shared resources and coordinates communication between nodes in the system.

Token Ring Algorithm: 
  - a token is passed around a logical ring topology, and only the node that holds the token is allowed to access a shared resource.
  - each philosopher communicates directly with their neighbors to acquire and release the shared resources.

To compile and execute program: 
  - make
  - ./dining_philosopher
