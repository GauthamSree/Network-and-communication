/**
    Code For :- Simulation of Go BACK-N ARQ protocol with different conditions.
    @file go_back_arq.cpp
    @author Gautham Sreekumar (GauthamSree)
*/

#include <math.h>
#include <iostream>
#include <thread>
#include <queue>
using namespace std;

#define TIMEOUT 10  // ACK TIMEOUT SIMULATION -- PUT 5 value to SIMULATE
#define revTIMEOUT 10 // Reciever ACK TIMEOUT SIMULATION -- PUT 5 value to SIMULATE
#define Corrupted 5 // CORRUPTED DATA SIMULATION -- PUT 5 value to SIMULATE 
#define TOT_PACKETS 8 // TOTAL PACKETS TO BE SEND 

typedef struct {
  int data;
} packet;

typedef struct {
  int seq;
  int ack;
  packet info;
} frame;

queue<frame> q, tmp;
static packet INFO;

queue<frame> DATA;
frame ACKNO;

typedef enum {RequestToSend, Frame_Arrival, Acknowledgement_Arrival, N_Ack, Timeout} event_type;

static int i = 1;    //Data to be sent by sender
int DISCONNECT = 0;
int timer = 0;
int cor = 0;
int revTimeout = 0;
int tot_pack_recieved = 0;
int tot_pack_made = 0;
int tot_pack_sent = 0;

int seq_Max;
int n;

event_type event;

bool Event(event_type e) {
  if(event == e) {
    return true;
  }
  return false;
} 

void WaitForEvent(int s) {
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

/// SENDER
void Get_Data_From_Network_Layer() {
  INFO.data = i;
  i++;
}

void Make_Frame(frame *s) {
  s->info = INFO;
  tot_pack_made += 1;
}

void Send_Frame(frame *s) {
  DATA.push(*s);
  tot_pack_sent++;                           
}

void Store_Copy_Frame(frame s) {
    q.push(s);
}

void Remove_copy_frame() {
    q.pop();
}

frame Retrieve_Copy_Frame() {
    return q.front();
}

void Receive_ACK(int *no) {
  *no = ACKNO.ack; 
}


// RECIEVER
void Receive_Frame_From_Physical_Layer(frame *r) {
  *r = DATA.front();
  DATA.pop();
}

void Send_ACK(int sqno) {
  ACKNO.ack = sqno;
}

void Deliver_Data_To_Network_Layer(frame r) {
  cout << "RECIEVER : Frame -> Seq No =  " << r.seq << ", Packet Data =  "<< r.info.data << " recieved  ";
  if (tot_pack_recieved > TOT_PACKETS) {
    DISCONNECT = 1;
  }
}

void check() {
  if (tot_pack_recieved >= TOT_PACKETS) {
    DISCONNECT = 1;
  }
}

void sender() {
  static int Sn = 0;
  cout << "Enter the no of bits in Sequence No: ";
  cin >> n;
  cout << endl;
  seq_Max = pow(2, n) + 1;
  static frame s;            // s denotes frame to be sent
  static frame t;            // t is temporary frame
  static int SeqFirst = 0;   // Sequence number of first frame in window
  static int SeqN = 0; 
  
  bool canSend = true;

  while(true) {
    WaitForEvent(0);
    check();
    if (DISCONNECT == 1) {
      event = Frame_Arrival;
      cout << "\n\nDISCONNECTED \n";
      break;
    }
    if(Event(RequestToSend)) {
      while((SeqN - SeqFirst) <= n && (tot_pack_made < TOT_PACKETS)) {
        Get_Data_From_Network_Layer();
        Make_Frame(&s);
        s.seq = (SeqN)%seq_Max;
        Store_Copy_Frame(s);
        cout << "SENDER : Sending Frame -> Seq No =  " << s.seq << ", Packet Data =  "<< s.info.data << "\n";
        Send_Frame(&s);
        SeqN = SeqN + 1;
      }
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
    WaitForEvent(1);
    if(Event(Acknowledgement_Arrival)) {
      int no;
      Receive_ACK(&no);      
      if(no == (SeqFirst + 1)%seq_Max) {
        cout << "SENDER : Ack no = " << no << " recieved\n"; 
        Remove_copy_frame();
        SeqFirst = SeqFirst + 1;
        tot_pack_recieved += 1;
        event = RequestToSend;
      }
    }
    if(Event(Timeout) || Event(N_Ack)) {
      if (Event(N_Ack)) {
        cout << "SENDER : N_ACK recieved \n";
      } 
      tmp = q;
      while (!tmp.empty()) {
        t = tmp.front();
        cout << "SENDER : Resending Frame -> Seq No =  " << t.seq << ", Packet Data =  "<< t.info.data << "\n";
        Send_Frame(&t);
        tmp.pop();
      } 
      event = Frame_Arrival;
      goto ack;
    } 
  }
}

void reciever() {
  static int Rn = 0;
  static frame r;
  while (true) {
    WaitForEvent(2);
    if (DISCONNECT == 1) {
          break;
    }
    if (cor == Corrupted) {
      cout << "RECEIVER : Data Corrupted & N_ACK sent\n";
      event = N_Ack;
      cor = 0;
    }
    if (Event(Frame_Arrival)) {
      if(!q.empty()) {
        Receive_Frame_From_Physical_Layer(&r);
        if (revTimeout == revTIMEOUT) {
          event = Timeout;
          revTimeout = -1;
          continue;
        }
        if (r.seq == Rn) {
          Deliver_Data_To_Network_Layer(r);
          Rn = (Rn + 1)%seq_Max;
          Send_ACK(Rn);
          event = Acknowledgement_Arrival;
        } else {
          cout << "RECIEVER : Frame -> Seq No =  " << r.seq << " Discarded \n";
        }
      } else {
        check();
      }
    }
  }
}

int main() {
  cout << "\nSimulation of Go BACK-N ARQ\n\nCONNECTED\n\n";
  event = RequestToSend;
  thread t1(sender);
  thread t2(reciever);
  t1.join();
  t2.join();
  cout << "\nTotal Packets send: " << tot_pack_sent << endl;
  return 0;
}
