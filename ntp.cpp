/// Author: Theodore Abohweyere

#include "ntp.h"
#include <cstring>
#include <bitset>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

const unsigned long long NTP_TIMESTAMP_DELTA = 2208988800ull; // NTP TIMESTAMP of 1 January 1970

void throwError(char *error)
{
    throw runtime_error(error);
}

int configureServerInfoAndSocket(struct sockaddr_in *serverAddress, struct hostent *serverData, int port)
{
    int sockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sockFd < 0)
        throwError("Could not open socket");

    if (serverData == NULL)
    {
        throwError("Unable to get server info. No such host");
    }

    bzero((char *)serverAddress, sizeof(serverAddress)); // Clear all the data from serverAddress

    serverAddress->sin_family = AF_INET;

    // copy the server's IP Address to the server address structure
    bcopy((char *)serverData->h_addr_list[0], (char *)&(serverAddress->sin_addr.s_addr), serverData->h_length);

    // Save port (network big-endian style) to serverAddress structure
    serverAddress->sin_port = htons(port);

    return sockFd;
}

int sendNTPPacket(int sockFd, struct sockaddr_in *serverAddress, NtpPacket *packet)
{
    if (connect(sockFd, (struct sockaddr *)serverAddress, sizeof(*serverAddress)) < 0)
        throwError("Unable to connect to server");

    // Send packet
    int result = write(sockFd, (char *)packet, sizeof(NtpPacket));

    return result;
}

int readNTPPacket(int sockFd, NtpPacket *receivedPacket)
{
    int result = read(sockFd, (char *)receivedPacket, sizeof(NtpPacket));

    if (result < 0)
        return result;

    // Convert data to host endianness
    receivedPacket->transmitTimeStampSeconds = ntohl(receivedPacket->transmitTimeStampSeconds);
    receivedPacket->transmitTimeStampFraction = ntohl(receivedPacket->transmitTimeStampFraction);

    return result;
}

int main(int argc, char **argv)
{
    // if (argc < 2)
    //     throwError("[server] argument is required");
    NtpPacket packet = {};

    // Zero out all the bits of the packet
    memset(&packet, 0, sizeof(NtpPacket));

    packet.leapVersionNumberMode = setMode(3, setVersionNumber(3, packet.leapVersionNumberMode));

    struct sockaddr_in serverAddress;

    struct hostent *serverData;
    char *hostName;

    if (argc < 2)
    {
        hostName = "ntp1.torix.ca";
    }
    else
    {
        hostName = argv[1];
    }
    int port = 123;

    serverData = gethostbyname(hostName); // Get server host info

    int sockFd = configureServerInfoAndSocket(&serverAddress, serverData, port);

    int result = sendNTPPacket(sockFd, &serverAddress, &packet);

    if (result < 0)
        throwError("Failed to send NTP packet");

    NtpPacket receivedPacket = {};

    int readResult = readNTPPacket(sockFd, &receivedPacket);

    time_t transmitTime = (time_t)(receivedPacket.transmitTimeStampSeconds - NTP_TIMESTAMP_DELTA);

    // Print out transmitTime in local time zone (convert from UTC)
    printf("Server Time: %s", ctime((const time_t *)&transmitTime));

    return 0;
}