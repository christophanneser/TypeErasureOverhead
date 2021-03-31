# Type Erasures and Virtual Function Call Overheads
In the conflict of sticking to OO-patterns and interfaces, and generic programming principles (such as templates), type erasure glues both worlds together. However, this comes at the cost of virtual function calls, which might not be acceptable in many contexts.

For a different research project, I optimized my approach for serial and parallel execution.
Therefore, I used different hash tables (a highly efficient hopscotch hash table and a parallel cuckoo hash map), which also brought slightly different APIs.

For the sake of clean code, I first preferred using an abstract class which would allow two choose one implementation at run-time depending on a parallel or serial execution.
In this case, type erasures for iterators (as described here: http://thbecker.net/free_software_utilities/type_erasure_for_cpp_iterators/any_iterator.html) offer a working solution to this problem.

However, type erasures come at the cost of virtual function calls, and this small benchmarks gives a very rough overview of the overhead I could expect for my research project.
