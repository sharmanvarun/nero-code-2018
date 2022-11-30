#include "mbed.h"
#include "MQTTEthernet.h"
#include "MQTTClient.h"
#define ECHO_SERVER_PORT   7
 Serial a(D1,D0);
    Serial s(USBTX, USBRX);

int arrivedcount = 0;
 
void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}

void baud(int baudrate) {
    s.baud(baudrate);
}

int main (void)
{
    
    baud(9600);
    printf("Wait a second...\r\n");
    char* topic = "Ultrasonic";
    MQTTEthernet ipstack = MQTTEthernet();
    
    MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
    
    char* hostname = "172.16.73.4";
    int port = 1883;
    
    int rc = ipstack.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\n", rc);
        
    printf("Topic: %s\r\n",topic);
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = "parents";

    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\n", rc);
 
    if ((rc = client.subscribe(topic, MQTT::QOS0, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc); 
        
    MQTT::Message message;
    char buf[100];
    while (true) 
    {
        char b;
        int i=0;
        char q[100];
        if(a.readable())
        { 
            for(i=0;i<=1;i++)
            {
            b= a.getc();
            q[i]=b;
            s.printf("%c",q[i]);
            }
            
        sprintf(buf,"%s",q);
        message.qos = MQTT::QOS1;
        message.retained = false;
        message.dup = false;
        message.payload = (void*)buf;
        message.payloadlen = strlen(buf);
        rc = client.publish("Ultrasonic", message);
        client.yield(2000);
    
            }
        
            }
}
