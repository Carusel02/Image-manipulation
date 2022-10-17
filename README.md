# Quaternary tree
### Implementation
- The most important part is the correct reading of the binary file and the correct transposition of the pixels in a ***matrix dynamically allocated***. 
- As a way of working, we need a main function processing that calculates the similarity score
and compares it with a certain threshold. 
- Then, calls are made according to the function (recursively) as many times as needed to divide the initial matrix in ***4 quarters*** and the information is stored each time in a tree.
- A function that transposes the elements from one tree to another is also necessary
vector and vice versa, the implementation consists of ***recursive calls***. 
- I did too 2 functions for specific writing of output files, and a function
which calculates the average value of a color channel.



