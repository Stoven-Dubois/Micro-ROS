# Micro-ROS

Middleware prototype, for managing the communication between different executable files

Language : C

Keywords: Middleware, Multi-Threading, Fastsim, C++

Data file : must be placed in "node_master", and follow the given structure:
  - node: "name_of_node"
  - bin: ./name_of_folder_with_executable_file
  - (optional) publish: "name_of_port" -> "name_of_fifo_file"
  - (optional) listen: "name_of_port" <- "name_of_fifo_file"
  - (optional) args: -- msg "message_to_print_when_node_is_activated" --Interval "period_of_node"
Some examples are placed in "node_master" ("config1.txt", "config2.txt").

Components: 
  - fastsim: fastsim library
  - node_echo: periodically writing to a port
  - node_logger: listening to a port
  - node_master: managing the whole process
  - node_simu: simulating a 2D-robot controlled through the keyboard
  - node_tele: writing keyboard commands to a port
  
Compilation : every makefile compiles all of the elements in the same folder, or in the sub-folders








