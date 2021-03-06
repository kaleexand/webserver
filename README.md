# webserv 

## About

Webserv is a project of the mandatory part of the cursus that I made with my friend [Helene Herin][2].
<br/><br/>It's only made in C++ and was about creating a web server able to handle one of the most famous web protocol, HTTP 1.1.
Our program is working with a configuration file, so you can setup the port, some routes, the allowed methods and some other basic paramaters for a webserver.
<br/>It's build based on UNIX sockets, and since it's an HTTP server you can access web pages using your favorite browser.

Our server handles the following HTTP features :
- The basics of HTTP protocol (status line / request line, a few headers, and payload).
- GET, POST and DELETE methods.
- HTTP response status codes (200, 404, ...).
- Error pages for the appropriate status code.
- CGI, like rendering a .php file in your browser thanks to php-cgi for example.

## Understanding the configuration file

Webserv need a configuration file that will be loaded at the beginning of the program, setting up all the necessary parameters. It works in a similar way than Nginx configuration file.
</br>It will allow you to setup the following parameters :
- `server` -- to define several virtual servers (with multiple server blocks).
- `server_name` -- to identify a specific server block thanks to host header.
- `listen` -- to configure on which port the server will be listening (only one `listen` per `server` block).
- `error` -- to set a path for custom error pages.
- `location` -- to set parameters for a specific route.
- `index` -- to serve a default file if none match the requested URI.
- `root` -- to route the requested URI to another directory.
- `upload_path` -- to set a specific directory for upload requests.
- `autoindex` -- to list all files present in the targeted directory.
- `allow_method` -- to define the allowed methods for HTTP requests.
- `cgi` -- to configure a cgi to execute for a specific file extenstion (php-cgi for .php files for example).

You can fing more examples in `conf` directory. `www` is a directory which allow you to test some cases / requests. If testing with your broser, use the browser*.conf files.

## Building and running the project

1. Download/Clone this repo

        git clone https://github.com/kaleexand/webserver.git

2. run `make`

        make

3.  Run `webserv` with a configuration file.
	
		./webserv name.conf

4.	Access web pages using your browser.

		# Example of a valid URI to request with a browser, with the config file from step 3
		http://localhost:8080/
