=====================================================================
				HOW TO BUILD AND RUN PROGRAM
=====================================================================
Step1. Build program
~~~~~~~~~~~~~~~~~~~~
cd to the source code folder then running make command
Note: The test_driver used CUnit testing framework
Before make, please install CUnit development framework
$ sudo apt-get install libcunit1-dev
$ make

After make success, it will generates following executable file.
test_driver -> The executable for test_driver
logserver 	-> The executable for the log server (for receiving log message)
logclient	-> The executable for the log client (for sending message to the logserve)
Step2. Run test_driver.
~~~~~~~~~~~~~~~~~~~~~~
$ ./test_driver

Step3. Launch server to waiting for receive message
Run the command
$. /logserve

Step4. Send the message from client to the server
Run the command on other terminal
$ ./logclient "test message"

At this time you can see the message "test message" display at the server's terminal.