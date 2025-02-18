1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use for/execvp instead of just calling execvp directly because it creates a new child process where the command can execute separately from the shell. Only calling execvp would replace the shell with the new command, and fork prevents this by using concurrent execution.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, an error message is printed to stderr and the shell executes the command and returns an error.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp() searches for the command in directories listed in the PATH environment variable. When execvp() is called the system searches for ls in each directory in PATH and returns -1 if not found.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  wait() ensures that the parent waits for the child process to finish before continuing. If we did not call it the child process would become a zombie process. 

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gets the exit code from a terminated process which tells us if a command returned an error or not so that invalid commands can return ERR_EXEC_CMD. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  the entire command line is stored in _cmd_buffer and creates a copy that we can modify it is then tokenized. This is necessary because splitting the command up would be different depending on if there is consideration of the quoted arguments. This lets the user input commands such as echo "cs 283" such that "cs 283" is echoed instead of taking in the command as "echo" "hello" "world".

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The main change I made to my parsing logic was replacing build_cmd_list() with build_cmd_buff() so commands and arguments are stored separately and helps with memory management. I also added functionality to handle arbitrary whitespace and to handle quoted arguments. There were no unexpected challenges in refactoring other than some difficulty with bugs after first implmenting.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals allow for a process to send and receive notifications asynchronously. They differ from other forms of interprocess communicaion because other IPCs because signals are immediate.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  Three commonly used signals include:
        1. SIGKILL (9) which forces a process to end immediately.
        2. SIGTERM (15) which gracefully exits a program. 
        3. SIGINT (2) which interupts a process from the keyboard.


- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP it pauses. It cannot be ignored because the kernel handles it which means it will always be processed.