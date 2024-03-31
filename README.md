<h1 align="center">C++ TCP Server</h1>

<p>This repository contains a multi-threaded TCP server implemented in C++. It's designed to handle multiple client connections simultaneously, with each client being served in a separate thread.</p>

<h2>🌟 Features</h2>

<ul>
  <li><strong>Multi-Threaded:</strong> Utilizes POSIX threads to handle each client connection in a separate thread.</li>
  <li><strong>Flexible:</strong> Can be easily modified and extended for various use cases involving TCP communication.</li>
  <li><strong>Simple Interface:</strong> Offers a straightforward way for clients to connect, send, and receive messages.</li>
</ul>

<h2>🚀 Getting Started</h2>

<p>These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.</p>

<h3>Prerequisites</h3>

<p>Ensure you have the following installed on your system:</p>
<ul>
  <li>g++</li>
  <li>POSIX-compatible operating system (Linux, macOS, etc.)</li>
</ul>

<h3>Compiling the Server</h3>

<p>To compile the server, navigate to the project directory and run the following command:</p>

<pre><code>g++ -o server main.cpp -pthread</code></pre>

<p>This will create an executable named <code>server</code> in your project directory.</p>

<h3>Running the Server</h3>

<p>To start the server, use the following command:</p>

<pre><code>./server &lt;port&gt;</code></pre>

<p>Replace <code>&lt;port&gt;</code> with the port number you wish the server to listen on.</p>

<h2>💡 Connecting to the Server</h2>

<p>Clients can connect to the server using any TCP client. For testing purposes, you can use <code>netcat</code>:</p>

<pre><code>nc localhost &lt;port&gt;</code></pre>

<p>Replace <code>&lt;port&gt;</code> with the same port number used when starting the server.</p>

<h2>🤝 Contributing</h2>

<p>Contributions are welcome! Please feel free to submit pull requests or open issues to discuss proposed changes or report bugs.</p>

