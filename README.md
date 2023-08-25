## Bism Ellah Elrahman Elraheem

### This is breakdown of webserve project requirement, will do our best to make all of us work on all aspects of the project to get the best learning outcome out of it:

- First : Pre starting the project let's do tutorials on building simple webserver in C language (all of us).
	- Intra [PDF](./documentation/webserve.pdf)
  	- Getting started toutorial [web serve](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
	- Writing a Simple TCP Server [Using Kqueue](https://dev.to/frosnerd/writing-a-simple-tcp-server-using-kqueue-cah)
	- Scalable I/O: [Events- Vs Multithreading-based](https://thetechsolo.wordpress.com/2016/02/29/scalable-io-events-vs-multithreading-based/)
	- Kqueue [tutorial](http://wiki.netbsd.org/tutorials/kqueue_tutorial/)
  	- Extensive resources [document](./documentation/webserve.md)
  	- [Beej guide for network programing](./documentation/resources/Beej_guide/bgnet_a4_c_2.pdf)
- Second: Here are the project requirments that can be assigned per person:
	1.	`Building the server`
After this step we should have more clarity what kind of data to be parsed and how to be stored and distriputed
	2.	`GET` (Salem)
	3.	`GET CGI` (Mahdy)
	4.	`POST` (Yassin)
	5.	`CGI POST`
	6.	`DELETE` (Salem)
	7.	`CGI DELETE` (Yassin)
	8.	`parsing requests` (Salem)
	9.	`parsing configurations`
	10.	`file uploads` (Mahdy)
	11.	`Chunking files` for sending request back (can be consider as part of get response)




----------------------------
#### Development iteration:
1. Develop the server with dummy response
    - Stress test the server
    - Decide on the what data structure will hold according to the server needs
2. Do Get request:
    - Try dummy request then serve the response
    - Parse the request according to RFC
    - Tinker hands with status codes
3. Post request:
    (same steps as GET)
4. DELETE reuquest:
    (same)
5. CGI GET
6. CGI POST
7. CGI DELETE
8. File uploads:
    - chunking RFC
(should have all required data to fill the map no inshalla)
9. Config files:
    - Enjoy parsing
    - test 
    - Merge with server
10. Handle signals

(only nesseccary step to start with is the server, all other steps can work parallel to each other)   
Thanks to https://github.com/straker for the amazing single page games.
