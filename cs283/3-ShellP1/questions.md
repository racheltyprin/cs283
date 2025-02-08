1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this application because it reads input line by line which is ideal for a shell that processes input one line at a time anyway. Because of how it reads characters, it ensures that entire line is captured. fgets() also handles EOF well.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: Using malloc() to allocate memory for cmd_buff allows for dynamic memory allocation, which is more flexible and efficient than a fixed-size array. A fixed size array would waste memory in cases where the input is smaller than the array size. Inversely, a fixed size array could have buffer overflow if the input is larger than the array size.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces from each command is necessary to ensure that commands and arguments are parsed correctly. Otherwise, some issues that may arise are commands with leading spaces may not be recognized as valid commands and trailing spaces can cause problems when comparing strings.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection allows us to control where the input and output of a command are sent. 3 redirection examples that we should implement in our custom shell are: 1. output redirection, 2. input redirection, and 3. appending STDOUT to output. Challenges associated with 1. are handling file permissions and proper file creation, 2. are ensuring files exist and permissions are correct, and 3. ensuring a file is open in append mode. 

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: The difference between redirection and piping is that redirection deals with files and devices, while piping connects commands directly.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: It is important to keep these separate in a shell so that debugging is easier as well as overall readability. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our shell should keep STDOUT and STDERR separate by default. In cases where a command outputs both STDOUT and STDERR, the user should be able to choose whether they merge or not. We could use 2>&1 to redirect STDERR to STDOUT