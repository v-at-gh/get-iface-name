#include <stdio.h>     /* for printf(), perror() */
#include <stdlib.h>    /* for free()*/
#include <string.h>    /* for strcmp(), strdup() */
#include <ifaddrs.h>   /* for ifaddrs, getifaddrs(), freeifaddrs() */
#include <arpa/inet.h> /* for INET_ADDRSTRLEN, AF_INET, sockaddr */
#include <signal.h>    /* for sigemptyset(), sigaction() */
#include <getopt.h>    /* for option, required_argument, getopt_long(), optarg */

#define SLEEP_INTERVAL 50000

volatile sig_atomic_t interrupted = 0;

void handle_sigint(int signum) {
	(void)signum;
	printf("\nScript interrupted. Exiting...\n");
	interrupted = 1;
}

char *find_iface_name(const char *ip_address) {
	struct ifaddrs *ifaddr, *ifa;
	char addr[INET_ADDRSTRLEN];
	char *iface_name = NULL;

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return NULL;
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) continue;
		if (ifa->ifa_addr->sa_family == AF_INET) {
			struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
			inet_ntop(AF_INET, &sa->sin_addr, addr, sizeof(addr));

			if (strcmp(addr, ip_address) == 0) {
				iface_name = strdup(ifa->ifa_name);
				break;
			}
		}
	}

	freeifaddrs(ifaddr);
	return iface_name;
}

void print_usage(const char *prog_name) {
	printf("Usage: %s [-a|--address <IP_ADDRESS>]\n", prog_name);
}

int main(int argc, char *argv[]) {
	char *ip_address = NULL;
	char *iface = NULL;
	struct sigaction sa;

	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	int opt;
	struct option long_options[] = {
		{"address", required_argument, 0, 'a'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "a:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'a':
				ip_address = strdup(optarg);
				break;
			default:
				print_usage(argv[0]);
				return 1;
		}
	}

	if (!ip_address) {
		print_usage(argv[0]);
		return 1;
	}

	while (!interrupted) {
		iface = find_iface_name(ip_address);
		if (iface != NULL) break;
		usleep(SLEEP_INTERVAL);
	}

	if (interrupted) {
		free(iface); free(ip_address);
		return 1;
	}

	printf("%s\n", iface);
	free(iface); free(ip_address);
	return 0;
}
