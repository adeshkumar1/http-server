HTTP Server in CThis is a simple, lightweight, and multi-threaded HTTP server implemented in C. It's designed to be a learning tool for understanding the basics of network programming, socket communication, and HTTP protocols.FeaturesMulti-threaded: Handles multiple client requests concurrently using POSIX threads.HTTP/1.1 Compliant: Supports basic HTTP/1.1 GET and HEAD requests.Serves Static Files: Can serve static files like HTML, CSS, JavaScript, and images.Directory Listing: If a directory is requested, it lists the contents.Customizable Port: The server can be started on a user-specified port.Cross-Platform: Written in standard C and should compile on most POSIX-compliant systems (Linux, macOS).Getting StartedThese instructions will get you a copy of the project up and running on your local machine for development and testing purposes.PrerequisitesYou'll need a C compiler (like gcc) and make installed on your system.# For Debian/Ubuntu
sudo apt-get update
sudo apt-get install build-essential

# For macOS (with Homebrew)
brew install gcc make
Installation & RunningClone the repository:git clone https://github.com/adeshkumar1/http-server.git
cd http-server
Compile the project:make
Run the server:By default, the server starts on port 8080../server
To specify a different port:./server <port_number>
For example, to run on port 9000:./server 9000
Access the server:
Open your web browser and navigate to http://localhost:8080 (or the port you specified). You should see the contents of the root directory. 
How It WorksSocket Creation: The server creates a TCP socket and binds it to the specified port. Listening: It then listens for incoming connections from clients. 
Accepting Connections: When a client connects, the server accepts the connection.Thread Creation: A new thread is created to handle the client's request, allowing the main thread to continue listening for other connections.
Request Parsing: The new thread reads the HTTP request from the client, parsing the method (GET, HEAD) and the requested path.
Response Generation:If the path is a file, the server sends the file's contents with the appropriate Content-Type header.If the path is a directory, it generates an HTML page listing the directory contents.
If the path is not found, it returns a 404 Not Found error.
Connection Closing: After the response is sent, the connection is closed, and the thread exits.
ContributingContributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.Fork the ProjectCreate your Feature Branch (git checkout -b feature/AmazingFeature)Commit your Changes (git commit -m 'Add some AmazingFeature')Push to the Branch (git push origin feature/AmazingFeature)Open a Pull RequestLicenseThis project is licensed under the MIT License - see the LICENSE file for details.
