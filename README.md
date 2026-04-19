# tcp-chat-application-c

# Multithreaded Chat System (C)

## 📌 Overview
This project is a client-server chat application built in C using TCP sockets and pthreads. It allows multiple clients to connect to a server, send messages, and receive real-time updates when users join, leave, or send messages.

## 🚀 Features
- Multi-client support using threads (pthreads)
- Real-time message broadcasting
- Join and leave notifications
- Structured message protocol
- Color-coded terminal output
- Client-server architecture using TCP sockets

## 🛠️ Tech Stack
- C
- POSIX Sockets
- Multithreading (pthreads)
- TCP/IP Networking

## ▶️ How to Run

### 1. Clone the repository
git clone https://github.com/your-username/multithreaded-chat-system-c.git

### 2. Build the project
make

### 3. Start the server
./server/server

### 4. Start clients (in different terminals)
./client/client

## 📡 How It Works
- Server listens for incoming connections
- Each client runs on a separate thread
- Messages are forwarded to all connected clients
- Custom message protocol handles:
  - Join events
  - Leave events
  - Message sending

## 🧠 What I Learned
- Socket programming in C
- Multithreading and synchronization (mutex)
- Client-server architecture
- Designing communication protocols
- Handling concurrency and race conditions

## 📌 Future Improvements
- Add private messaging
- Support remote connections (not just localhost)
- Add authentication system
- Improve error handling and logging

## 👤 Author
Mena Mikhail
