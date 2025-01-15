/*
 * Simple MQTT Client for PDP-11/83 to Report CPU Usage
 * Written in ANSI C (K&R compatible) for 211BSD
 * Implements MQTT CONNECT with Username and Password Authentication
 * Parses command-line options using getopt()
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     /* For getopt() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  /* For inet_addr */

/* Define default MQTT broker details */
#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT     1883
#define DEFAULT_TOPIC    "pdp11/cpu_usage"
#define DEFAULT_VALUE    "42.5%"
#define CLIENT_ID        "pdp11"

/* Function prototypes */
int connect_to_broker(char *broker_ip, int port);
int send_mqtt_connect(int sock, char *username, char *password);
int receive_connack(int sock);
int publish_message(int sock, char *topic, char *message);
int send_disconnect(int sock);
int encode_remaining_length(int length, unsigned char *buffer);
int send_all(int sock, unsigned char *buffer, int length);
int NumberOfUsers(void);
int CPUUsage(void);

/* Custom memmove implementation if bcopy is unavailable */
#ifndef HAVE_BCOPY
void my_memmove(char *dest, char *src, int n) {
    int i;
    if (src < dest) {
        for (i = n - 1; i >= 0; i--) {
            dest[i] = src[i];
        }
    } else {
        for (i = 0; i < n; i++) {
            dest[i] = src[i];
        }
    }
}
#endif

int main(int argc, char *argv[]) {
    int sock;
    char *hostname;
    char *username = "";
    char *password = "";
    char *topic = DEFAULT_TOPIC;
    char *value = DEFAULT_VALUE;
    int rc;
    int opt;
    extern char *optarg;
    extern int optind;

    /* Check if no arguments are provided */
    if (argc == 1) {
        fprintf(stderr, "Usage: %s [-h hostname] [-u username] [-p password] [-t topic] [-v value]\n", argv[0]);
        exit(1);
    }

    /* Initialize hostname to default */
    hostname = DEFAULT_HOSTNAME;

    /* Parse command-line options */
    while ((opt = getopt(argc, argv, "h:u:p:t:v:")) != EOF) {
        switch (opt) {
            case 'h':
                hostname = optarg;
                break;
            case 'u':
                username = optarg;
                break;
            case 'p':
                password = optarg;
                break;
            case 't':
                topic = optarg;
                break;
            case 'v':
                value = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-h hostname] [-u username] [-p password] [-t topic] [-v value]\n", argv[0]);
                exit(1);
        }
    }

    /* Ensure mandatory arguments are provided */
    if (username[0] == '\0' || password[0] == '\0') {
        fprintf(stderr, "Error: Username and password are required.\n");
        fprintf(stderr, "Usage: %s [-h hostname] [-u username] [-p password] [-t topic] [-v value]\n", argv[0]);
        exit(1);
    }

    /* Connect to MQTT broker */
    sock = connect_to_broker(hostname, DEFAULT_PORT);
    if (sock < 0) {
        fprintf(stderr, "Failed to connect to MQTT broker.\n");
        exit(1);
    }

    /* Send MQTT CONNECT packet */
    rc = send_mqtt_connect(sock, username, password);
    if (rc != 0) {
        fprintf(stderr, "Failed to send MQTT CONNECT packet.\n");
        close(sock);
        exit(1);
    }

    /* Receive CONNACK packet */
    rc = receive_connack(sock);
    if (rc != 0) {
        fprintf(stderr, "Failed to receive valid CONNACK from broker.\n");
        close(sock);
        exit(1);
    }

    /* Publish the message */
    rc = publish_message(sock, topic, value);
    if (rc != 0) {
        fprintf(stderr, "Failed to publish message.\n");
        close(sock);
        exit(1);
    } else {
        printf("Published: %s\n", value);
    }

    /* Send MQTT DISCONNECT packet */
    rc = send_disconnect(sock);
    if (rc != 0) {
        fprintf(stderr, "Failed to send DISCONNECT packet.\n");
    }

    /* Close the socket */
    close(sock);

    return 0;
}
