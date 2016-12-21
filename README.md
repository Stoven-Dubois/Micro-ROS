# Micro-ROS

Micro version of ROS, parsing a data file, and creating nodes for the communication between executable files

Language : C

Used Tools : FIFO, multi-threading, mutex, fastsim library, Visual Studio

Data file : has to be placed in "node_master" and follow the structure shown under.
  - node: "name_of_the_node"
  - bin: ./name_of_the_folder_containing_executable_file
  - (optional) publish: "name_of_the_port" -> "name_of_the_fifo_file"
  - (optional) listen: "name_of_the_port" <- "name_of_the_fifo_file"
  - (optional) args: -- msg "message_to_print_when_node_is_activated" --Interval "period_of_node"
Some examples are placed in "node_master" ("config1.txt", "config2.txt").

Components: 
  - fastsim: fastsim library
  - node_echo: node periodically writing a message to a port
  - node_logger: node listening to a port
  - node_master: node leading the program's behavior
  - node_simu: simulation of a 2D-robot controlled through the keyboard
  - node_tele: node writing keyboard commands to a port








