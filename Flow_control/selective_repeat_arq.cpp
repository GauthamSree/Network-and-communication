/**
    Code For :- Simulation of Selective Repeat ARQ protocol with different conditions.
    @file selective_repeat_arq.cpp
    @author Gautham Sreekumar (GauthamSree)
*/

#include <math.h>
#include <iostream>
#include <thread>
#include <queue>
#include <list>
#include<iterator> 
#include <map>

using namespace std;

#define TIMEOUT 10   // ACK TIMEOUT SIMULATION -- PUT 5 value to SIMULATE
#define Corrupted 10  // CORRUPTED DATA SIMULATION -- PUT 5 value to SIMULATE 
#define TOT_PACKETS 8 // TOTAL PACKETS TO BE SEND 

typedef struct {
  int data;
} packet;

typedef struct {
  int seq;
  int ack;
  int sseq;
  packet info;
} frame;

list<frame> q, tmp;
static packet INFO;

queue<frame> DATA;
frame ACKNO;
frame NACK;
map<int, int> recieved;
typedef enum {RequestToSend, Frame_Arrival, Acknowledgement_Arrival, N_Ack, Timeout} event_type;

static int i = 1;    //Data to be sent by sender
int DISCONNECT = 0;
int timer = 0;
int cor = 0;
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
    q.push_back(s);
}

void Remove_copy_frame(int no) {
  for (list<frame>::iterator m = q.begin(); m!=q.end(); m++) {
    if((*m).seq == no) {
      q.erase(m);
      return;
    }
  } 
}

void Remove_copy_frame_from_front() {
  q.pop_front();
}

frame Retrieve_Copy_Frame() {
    return q.front();
}

void Receive_ACK(int *no) {
  *no = ACKNO.ack; 
}

void Receive_NACK(int *no) {
  *no = NACK.ack; 
}


// RECIEVER
void Receive_Frame_From_Physical_Layer(frame *r) {
  *r = DATA.front();
  DATA.pop();
}

void Send_ACK(int sqno) {
  ACKNO.ack = sqno;
}

void Send_NACK(int sqno) {
  NACK.ack = sqno;
}

void check() {
  if (tot_pack_recieved >= TOT_PACKETS) {
    DISCONNECT = 1;
  }
}

void Deliver_Data_To_Network_Layer(frame r) {
  cout << "RECIEVER : Frame -> Seq No =  " << r.seq << ", Packet Data =  "<< r.info.data << " recieved  ";
  check();
}

void find_frame(int seqno, frame *t) {
  for(const frame& lis: q) {
    if(lis.seq == seqno) {
      *t = lis;
    }  
  }
}

void sender() {
  static int Sn = 0;
  cout << "Enter the no of bits in seqno: ";
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
      while(q.size() <= n && (tot_pack_made < TOT_PACKETS)) {
        Get_Data_From_Network_Layer();
        Make_Frame(&s);
        s.seq = (SeqN)%seq_Max;
        s.sseq = SeqN;
        Store_Copy_Frame(s);
        cout << "SENDER : Sending Frame -> Seq No =  " << s.seq << ", Packet Data =  "<< s.info.data << "\n";
        Send_Frame(&s);
        SeqN = SeqN + 1;
      }
      event = Frame_Arrival;
    }
    timer++;
    cor++;
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
        Remove_copy_frame(SeqFirst);
        SeqFirst = SeqFirst + 1;
        tot_pack_recieved += 1;
        event = RequestToSend;
      } else {
        cout << "SENDER : Ack no = " << no << " recieved\n"; 
        Remove_copy_frame((no - 1 + seq_Max)%seq_Max);
        tot_pack_recieved += 1;
        event = RequestToSend;
      }
    }
    if(Event(N_Ack)) {
      int no;
      Receive_NACK(&no);
      cout << "SENDER : N_ACK recieved for Seq No " << no << "\n";
      find_frame(no, &t);
      cout << "SENDER : Resending Frame -> Seq No =  " << t.seq << ", Packet Data =  "<< t.info.data << "\n";
      Send_Frame(&t);
      event = Frame_Arrival;
      goto ack;
    }
    if(Event(Timeout)) {
      t = Retrieve_Copy_Frame();
      Remove_copy_frame(t.seq);
      cout << "SENDER : Resending Frame -> Seq No =  " << t.seq << ", Packet Data =  "<< t.info.data << "\n";
      Store_Copy_Frame(t);
      Send_Frame(&t);
      event = Frame_Arrival;
      goto ack;
    }  
  }
}

void reciever() {
  static int Rn = 0, tmp = 0;
  static frame r;
  while (true) {
    WaitForEvent(2);
    if (DISCONNECT == 1) {
          break;
    }
    if (Event(Frame_Arrival)) {
      if(!q.empty()) {
        Receive_Frame_From_Physical_Layer(&r);
        if (cor == Corrupted) {
          cout << "RECEIVER : Data Corrupted & N_ACK for SeqNo "<< r.seq << " sent\n";
          Send_NACK(r.seq);
          event = N_Ack;
          cor = 0;
          continue;
        }
        if (r.seq == Rn && (recieved.find(r.sseq) == recieved.end())) {
          Deliver_Data_To_Network_Layer(r);
          recieved[r.sseq]++;
          Rn = (Rn + 1)%seq_Max;
          Send_ACK(Rn);
          event = Acknowledgement_Arrival;
        } else {
          if((recieved.find(r.sseq) != recieved.end())) {
            event = Acknowledgement_Arrival;
            continue;
          }
          Deliver_Data_To_Network_Layer(r);
          recieved[r.sseq]++;
          tmp = (r.seq + 1)%seq_Max;
          Send_ACK(tmp);
          event = Acknowledgement_Arrival;
        }
      } else {
        check();
      }
    }
  }
}

int main() {
  cout << "\nSimulation of Selective Repeat ARQ\n\nCONNECTED\n\n";
  event = RequestToSend;
  thread t1(sender);
  thread t2(reciever);
  t1.join();
  t2.join();
  cout << "\nTotal Packets send: " << tot_pack_sent << endl;
  return 0;
}
