# netcloak

**netcloak** is a lightweight command-line tool written in C for Linux that allows users to manipulate MAC (Media Access Control) addresses on network interfaces. It supports changing, restoring, printing, and randomizing MAC addresses, as well as bringing interfaces up or down. This tool interacts directly with Linux networking APIs (`ioctl`, `ethtool`) to provide robust and low-level control over network interface configuration.  

---  

## Features  

- Change MAC address to a user-specified or random address  
- Restore the original (permanent) MAC address using `ethtool`  
- Print current or permanent MAC addresses  
- Bring network interfaces up or down  
- List all available network interfaces  
- Validate MAC address input to ensure correctness  
- User-friendly CLI with short and long options, and helpful error messages  

---  

## Requirements  

- Linux operating system  
- C compiler (e.g., gcc)  
- Root privileges (most operations require elevated permissions)  
- Development headers for networking (`linux/ethtool.h`, `net/if.h`, etc.)  

---  

## Building  

Clone the repository and run `make` to build the binary:  

```bash  
git clone https://github.com/younggoat34/netcloak.git  
cd netcloak  
make all  

```  

