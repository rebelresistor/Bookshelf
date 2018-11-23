
// the stock version of WiFiServer has a bug/feature where
// it complies with the TCP spec to the point of crashing the
// chip. It retains 180 bytes per connection for 3 mins in
// case the client wants to reconnect. The tcpCleanup() function
// below aborts all remaining TCP connections in the history
// and recovers the memory.

// this is useful to run at the end of loop(), to recover any memory
// lost by closed connections hanging around.

// stolen from: https://gist.github.com/d-a-v/ed67f7a6f476a043d1c7f347c829087e
struct tcp_pcb;
extern struct tcp_pcb* tcp_tw_pcbs;
extern "C" void tcp_abort (struct tcp_pcb* pcb);

void tcpCleanup ()
{
  while (tcp_tw_pcbs != NULL)
  {
    tcp_abort(tcp_tw_pcbs);
  }
}

