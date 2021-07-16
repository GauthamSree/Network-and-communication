/**
    Code For :- Simulation of Stop and Wait ARQ protocol with different conditions.
    @file stop_and_wait_arq.cpp
    @author Gautham Sreekumar (GauthamSree)
*/

#include <iostream>
#include <thread>

using namespace std;

#define TIMEOUT 10  // ACK TIMEOUT SIMULATION -- PUT 5 value to SIMULATE
#define revTIMEOUT 10 // Reciever ACK TIMEOUT SIMULATION -- PUT 5 value to SIMULATE
#define Corrupted 10 // CORRUPTED DATA SIMULATION -- PUT 5 value to SIMULATE 
#define TOT_PACKETS 8 // TOTAL PACKETS TO BE SEND 

typedef struct {
  int data;
} packet;

typedef struct {
  int seq;
  int ack;
  packet info;
} frame;

frame DATA;
frame ACKNO;
static int ackno = 0;

typedef enum {RequestToSend, Frame_Arrival, Acknowledgement_Arrival, N_Ack, Timeout} event_type;

static int i = 1;         // Data to be sent by sender
int DISCONNECT = 0;
static int timer = 0;
static int cor = 0;
static int revTimeout = 0;

event_type event;

bool Event(event_type e) {
  if(event == e) {
    return true;
  }
  return false;
} 

void WaitForEvent1(int s) {
  if (s == 0) {
    while (true) {
      if (Event(RequestToSend)) {
        break;
      }
    }
  }
  if (s == 1) {
    while (true) {
      if (Event(Acknowledgement_Arrival) || Event(Timeout) || Event(N_Ack)) {
        break;
      }
    }
  }
  if (s == 2) {
    while (true) {
      if (Event(Frame_Arrival)) {
        break;
      }
    }
  }
  return;
}

void from_network_layer(packet *buffer) {
  (*buffer).data = i;
  i++;
}
void to_physical_layer(frame *s) {                        
  DATA = *s;      
}
void from_physical_layer(frame *buffer) {
  *buffer = DATA;
}

void SendACK() {
  ackno = 1 - ackno;
  ACKNO.ack = ackno; 
  cout << "Ack no = " << ackno << " sent \n"; 
}
void recieveACK(int *no) {
  *no = ACKNO.ack; 
}

void to_network_layer(packet *buffer) {
  if(revTimeout == -1) {
    cout << "RECIEVER : Packet = " << (*buffer).data << " recieved (Duplicate)  ";
  } else {
    cout << "RECIEVER : Packet = " << (*buffer).data << " recieved  ";
  }
  SendACK();
  if (i > TOT_PACKETS) {
    DISCONNECT = 1;
  }
}

void Make_Frame(int frametosend, packet *buffer, frame *s) {
  s->info = *buffer;
  s->seq = frametosend;
  cout << "SENDER : Info =  "<< s->info.data << "     Seq No = " << s->seq << endl;
}

void sender() {
  packet buffer;
  static frame s;
  static int Sn = 0;
  
  bool canSend = true;
  while(true) {
    WaitForEvent1(0);
    if (DISCONNECT == 1) {
      cout << "\nDISCONNECTED\n";
      break;
    }
    if (Event(RequestToSend) && canSend) {
      from_network_layer(&buffer);
      Make_Frame(Sn, &buffer, &s);
      to_physical_layer(&s);
      Sn = 1 - Sn;
      canSend = false;
      event = Frame_Arrival;
    }
    timer++;
    cor++;
    revTimeout++;
    if (timer == TIMEOUT) {
        event = Timeout;
        cout << "SENDER : Ack not recieved => TIMEOUT\n";
        timer = 0;
    }
    ack:
    WaitForEvent1(1);
    if (Event(Acknowledgement_Arrival)) {
      int no;
      recieveACK(&no);
      if (ACKNO.ack == no) {
        cout << "SENDER : Ack no = " << ackno << " recieved\n"; 
        canSend = true;
        event = RequestToSend;
      } 
    }
    if (Event(Timeout) || Event(N_Ack)) {
      if (Event(N_Ack)) {
        cout << "SENDER : N_ACK recieved \n";
      } 
      cout << "SENDER : Resending Frame " << "(Info =  "<< s.info.data << ")\n";
      to_physical_layer(&s);
      event = Frame_Arrival;
      goto ack;
    }
  }
}

void reciever() {
  static int Rn = 0;
  static frame r;
  while (true) {
    WaitForEvent1(2);
    if (cor == Corrupted) {
      cout << "RECEIVER : Data Corrupted & N_ACK sent\n";
      event = N_Ack;
      cor = 0;
    }
    if (Event(Frame_Arrival)) {
      from_physical_layer(&r);
      if (revTimeout == revTIMEOUT) {
        to_network_layer(&r.info);
        event = Timeout;
        cout << "SENDER : Ack not recieved => TIMEOUT\n";
        revTimeout = -1;
        continue;
      }
      if (r.seq == Rn) {
        to_network_layer(&r.info);
        Rn = 1 - Rn;
        event = Acknowledgement_Arrival;
        if (DISCONNECT == 1) {
          break;
        }
      } 
    }
  }
}

int main() {
  cout << "\nSimulation of STOP AND WAIT ARQ\n\nCONNECTED\n\n";
  event = RequestToSend;
  thread t1(sender);
  thread t2(reciever);
  t1.join();
  t2.join();
  return 0;
}