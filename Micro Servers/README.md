<h1>Micro Servers</h1>

Written by: Lauren Mayes

Assignment given by: Dr Carey Williamson & Dr. Sina Keshvadi

This was an assignment from Introduction to Networks at the University of Calgary. 
We were taked with setting up a a client, 3 micro servers, and 1 indirection server that acts as a proxy between the client and micro servers. 
In this project I demonstrate socket programming by creating and using client and server sockets to send, receve and parse data packets. 

How to use:
	1) Compile and run the micro servers ("CurrencyServer.c", "TransServer.c", "VotingSerevr.c") with "gcc -o", on server 1. 
	2) Then compile and run the indirection server ("indirection.c"), on server 2.
	3) Then run the client ("Client.c"), on server 2.
	4) Only type into the client command line and follow the directions on screen.

What works:
-	The TCP connection and UDP connections work, through all my testing I never experienced problems with dropped packets, though it may still be possible.
-	The loops with the translation server and the conversion server work well, no bugs were found with testing.
Known bugs:
-	Exact user input is necessary, wrong input can cause the servers to crash.
-	Some commands with voting will cause the servers to freeze and stop sending a receiving packets.
