#Minishell 🚀  

In this project we have to code a shell in C language. Concretely we tried to replicate bash.  

For this project I worked wit my dear alvapari (check his github here: https://github.com/alvapari).  

In order to code our Minishell we had to handle the commands parsing and the commands execution. We created a useful data structure that stores the commands as a list.  

The arguments for the commands are stored in an array on each node and the redirections are stored as a list inside each node too.  

So this way we can execute the command lists and recreate the pipes between them.  

We had to work with our own copy of enviroment variables and we had to modify it every time we must to.  

Also we had to make our own builints for specific commands (echo, cd, pwd, export, unset, env and  exit).  

We had to manage the file descriptors so everything works as it should with he redirections.  

We had to manage our exit and error codes so we replicate bash.  

And finally we had to manage the signals and the history so our Minishell works in a similar way to bash.  


- Allowed functions: readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs.  


##Topics & Learnings  

- Deep shell and bash learning
- Deep parsing including simple and double quotes
- Deep data structures managing
- Deep file descriptors managing for multiple redirections
- Deep execution and processes managing including pipes for multiple commans execution
- Terminal interaction
- Signals catching and triggering
- History managing
- Exit and error codes managing and replication
- Environment variables usage and modification
