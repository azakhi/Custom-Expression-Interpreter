# Custom-Expression-Interpreter
A custom expression class that lets defining of custom operations


Supports:
 - Positive and negative numbers (Note that '-' must be followed by the number when writing negative numbers and it will always interpret such combination a negative number)
 - Different priorities between operations. In case of equal priorities left operation will be calculated first.
 - Parantheses (highest priority, must be matching)
 - Spaces are used to seperate numbers and operations. Amount of spaces in between does not matter. Any other character other than space and numbers is considered as an operation
 - Custom operation with any amount of characters. Operations must be requiring 2 inputs and be written in between them.
 - Custom priorities of operations
 
 Note that operation handler must be provided.
 
 Check basicmath.cpp for example usage.
