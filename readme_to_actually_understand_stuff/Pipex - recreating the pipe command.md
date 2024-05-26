# Pipex - recreating the pipe command

*I don’t know if I enjoyed this project because it interested me on an intellectual level, or if it was due to the fact that I could google “how to create a child” without feeling weird.* 


If you’re new to pipex you may have at least one raised eyebrow right now. Let me lower them for you. 

This is a semi-detailed tutorial of the pipex project, whose main purpose is recreating the properties of the “pipe” command in bash. 

*Lowered?*

![image](https://github.com/Varaunevik/Pipex/assets/145858191/e23490ce-550d-4676-b352-ea6d0c1dcb99)


---

## General overview:

```c
./pipex infile cmd1 cmd2 outfile

//should behave like:

< infile cmd1 | cmd2 > outfile
```

**We want to do the following:**

1. Read from **infile**
2. Execute **cmd1** with **infile** as input
3. Send the **output** of **cmd1** to **cmd2** through a pipe
4. **Write** the **output** of **cmd2** to the **outfile**

I would recommend playing a bit around with the pipe command to see how it works with different commands and permissions for the infile/outfiles. 

---

## Child labor (and why we love it)

*So, this is where we begin our long journey of exploitation.* 

In order for our program to handle multiple commands and pipes at once, we have to create multiple processes. 

These processes will be **duplicates of the parent function**, aka. their children. 

Each child will be in charge of one command. For reading input, executing the command, and then writing the output to the correct file descriptor. 

It will also make sure to close and open the proper ends of the pipe. 

### But how does one make children?

Forget birds and bees, let’s get down to the real deal:

**You make a child using a fork().**

I think one of the most important parts of this project is properly understanding what a fork is and how to use it. 

*Don’t worry though, is not a tutorial in dinner etiquette.* 

**Typically, the parent will create a child process simply by calling fork().** 

This creates a clone of the parents memory state and file descriptors. Thus the child ends up with an **independent copy of the variables and file descriptors** that existed in the parent at the time of the call. 

Just like you inherited your parents DNA at the time of…uhm creation. 

If your mother, after your birth, contracts malaria, does not automatically mean you will get malaria.

If your parent function declares a variable after creating a child using fork() does not give the child the same variable. It is simply too late.

Parent and child are now separate entities.

In the parent process, an array is created to store TWO file descriptors. Fd[2]. 

After the pipe() call, the array is filled with the correct fds. The first one will be read from the child process, and the second will be written to by the parent process *(if multiple commands, children will communicate with each other in the same way).* 

Now that the child’s stdin is ready to read data, the child closes the file descriptors created by the pipe() call since they are no longer needed in the child. The child then executes the command.

After the fork() call, the parent will close the fd[0], since it is not needed in the parent (the parent only needs to write data, not read it). 

The parent then writes everything to the writable end of the pipe (fd[1]). When finished, the parent also closes fd[1], since it is done writing data. 

This will send an EOF (end of file) to the child, which is its clue to execute the command. 

**TLDR;**

1. The **infile will be the input** for the **first command** to handle. 
2. The **result of this command on the input will be written to the fd[1]** (the write-only end of our pipe).
3. The result will then *“travel”* through the pipe’s **read-only end (fd[0])**. What was once output is now **input for the second command** to handle. Which is why we only need write permission.
4. The **command executes on the input** and again produces output which now will be **written to the standard output** file (outfile).

![image](https://github.com/Varaunevik/Pipex/assets/145858191/cc96983b-ee1e-420b-b9b6-9643a3fc570b)

That is the main idea of the pipex project.

### MANY CHILDREN WITH MANY FORKS!

The bonus part wants us do do this with multiple commands and pipes. It is essentially the same, only with more file descriptors and processes. 

**Now, the commands needs to be executed by someone.** Unpopular opinion, but child labor is actually a great option. 

**We create one child process for each command.** They will follow the pipeline, opening and closing file descriptors as they finish. 

![image](https://github.com/Varaunevik/Pipex/assets/145858191/1edfcd65-7f68-484a-93a3-190eb2b45ff6)

This is an attempt at visualizing the pipex program when we have multiple commands and child processes.

### Waiting for the children

We have to use the wait() function in order to wait for the child processes to finish before returning. 

Since we have many child processes, we also need to wait many times. Actually, we have to wait the amount of children we have. 

This is pretty easy - we just call wait() in a loop.

**Here is why it works:**

1. Whenever you call fork(), a new child is created. The parent received the child’s PID, and the child receives a return value of 0 from fork(). 
2. As the parent, when you call wait(), the operating system puts the parent process to sleep until any of its child processes terminate. 
3. So - when a child terminate, the operating system wakes up the parent and wait() returns the PID of the terminated child process.
4. You can then use this PID to determine which child process has terminated and retrieve its exit status.
5. If there are multiple children, you typically call wait() in a loop until all children have terminated. The wait will return the PID of the next terminated child process each time its called. 

So, you don't need to explicitly specify which child process to wait for. **The operating system keeps track of child processes and manages their termination and exit status retrieval.**

---

## Finding the correct path

Okay - I have been speaking about “executing the command” like it is some sort of witchery that just happens. 

Unfortunately though, we have to do quite a bit in order to even find the correct path to the command we want to execute. 

You may be used to the general structure of the main function, looking something like this:

```c
int main(int argc, char **argv);
```

However, in this project, we will take use of one more variable in our main - the environmental variables. 

```c
int main(int argc, char **argv, char **envp);
```

The envp is the list of character strings. If you write env in your terminal, you’ll be able to better understand what I am talking about. 

Each line is stored as a string in a matrix, similarly to the argument vectors. 

There is really only one line in the env we are interested in, and you may have guessed it by now. 

**The “PATH=” line.** 

It specifies the directories to be searched to find a command. 

Which is exactly what we want to do. We want to find the correct command. 

The paths are divided by “:”, not a space. Sort of like this (look for the line after PATH):

![image](https://github.com/Varaunevik/Pipex/assets/145858191/82fb3056-6d8c-46a6-887a-4e977f7a87a1)

### Step-by-step to find the correct path

1. Search for the PATH= line in the envp matrix by iterating through each line. You want to compare (strcmp or similar functions from libft) the first 5 letters of each string with the string “PATH=”. If they match, we have our correct string. 
2. We now want to split the line we just found by using ft_split with “:” as delimiter. This will give us all the possible paths stored in a matrix. However, make sure that you only start splitting from the fifth letter, as we don’t want the “PATH=” characters stored in the matrix. 
3. Now we have a 2D matrix with all our paths stored individually. Unfortunately we are not yet finished. The paths are incomplete. 
4. For a path to be complete, it has to end with a “/” character. This will prove useful later when we want to check the existence of the command using the access() function. 
5. You want to iterate through the matrix, use strjoin with the current string and “/”. Then you want to store this new string in the old position and free the original path that was incomplete. 
6. After doing this you finally have all complete paths stored in a 2D matrix in your structure! Congratulations.

---

## Error messages and return values

I will be honest and say that this is definitely my least favorite part of the program. 

The project pdf is being annoying (what’s new?), and tells us only that “when in doubt, just handle errors like the bash command”.

This essentially means we have to be VERY pedantic when writing our exit functions.

### Allowed return/error functions

**Perror()**

Prints error message corresponding to the value of errno and then a description of the error encountered. 

**Strerror()**

Obtains a string that describes a given error. Returns a pointer to an error message string that corresponds to the specified error number. 

**Exit()**

Will exit the program, terminate all the programs that are currently running, flush all file buffers and close all stream and delete temporary files. 

**Some error checks you may not think about:**

- Command is written with path (such as /usr/bin/grep)
- Command is not a directory
- Program exit codes (write echo $?)
- First file does not exist/do not have read rights
- Empty command (” “)
- First command not found/second/both
- All errors at once (such as nofile “ca” “ “ /dev/null/ ) then check echo $?

---

Now that we have some background on the program and its functions - let’s start coding. 
