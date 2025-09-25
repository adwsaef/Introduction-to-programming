# Office Queue Simulation — Project Summary

The goal is to simulate **events during a working day** in an office with `m` service windows (0 … m-1).  
Each window has its own queue of clients. Clerks serve clients in order, but during the day various operations may happen that affect the queues.

---

## Functions to Implement

### Initialization
- **`void otwarcie_urzedu(int m)`**  
  Called once at the start. Informs that the office has `m` windows (plus one special window). Initially, there are no clients.

---

### Clients
- **`interesant* nowy_interesant(int k)`**  
  A new client enters and joins the queue at window `k`. Returns a pointer to the created client.

- **`int numerek(interesant* i)`**  
  Returns the unique ID number of client `i` (starting from 0).

---

### Serving
- **`interesant* obsluz(int k)`**  
  Clerk at window `k` serves the first client in the queue (if any). Returns pointer to that client or `NULL` if the queue is empty.

---

### Queue Manipulation
- **`void zmiana_okienka(interesant* i, int k)`**  
  Client `i` leaves their current queue and joins the end of queue at window `k`.

- **`void zamkniecie_okienka(int k1, int k2)`**  
  Window `k1` closes temporarily. All clients from queue `k1` are moved (in the same order) to the end of queue `k2`.

---

### Special Operations
- **`std::vector<interesant*> fast_track(interesant* i1, interesant* i2)`**  
  A special window opens briefly. All clients from `i1` to `i2` (consecutive in one queue) are instantly served in the same order. Returns the list of served clients.

- **`void naczelnik(int k)`**  
  The boss reverses the order of the queue at window `k`. If 0 or 1 clients are present, nothing changes.

---

### Closing
- **`std::vector<interesant*> zamkniecie_urzedu()`**  
  Called once at the end. All remaining clients are served in order:  
  - First, everyone from queue 0,  
  - then queue 1,  
  - and so on.  
  Returns a vector of all served clients in that order.
