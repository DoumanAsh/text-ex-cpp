# Some C++ text exercise

## Task

Create a c++ class that can store relatively big amounts (around 100 000) of an arbitrary type
of object.

1. The class should be able to associate each stored object with an integer "weight". If no
weight is provided by a user at the time of adding the object, a default weight is
associated with the object.
2. The class should be able to provide a list of items of a specified weight. The class should
also be able to provide a list of used weights.
3. The class should have a method for executing an arbitrary function (provided by a user),
which takes a stored object type as a parameter, sequentially on all stored objects in the
ascending order of associated weights. E.g. object with an associated weight of 0 will be
passed to the function before the object with an associated weight of 1.
4. The class should have a method for altering associated object weights. This method takes
a function (provided by a user) as a parameter, said function takes an object and it's
current weight and returns the new weight. The class should reassociate it's object
weights based on that function. E.g. an object with an associated weight of 2 after the
method call can have an associated weight of 32, based on the user-provided function.


