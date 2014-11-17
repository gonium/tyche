package main

import (
	"bufio"
	//"bytes"
	//"encoding/bytes"
	"encoding/hex"
	"fmt"
	"github.com/tarm/goserial"
	"os"
	"strings"
)

func main() {
	c := &serial.Config{Name: "/dev/cu.usbserial-A600e1gh", Baud: 115200}
	serial_conn, err := serial.OpenPort(c)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Cannot open device: "+err.Error())
	}

	f, err := os.Create("rnglog")
	if err != nil {
		fmt.Fprintf(os.Stderr, "Cannot create file: "+err.Error())
	}
	defer f.Close()

	//buf := make([]byte, 128)
	//n, err := s.Read(buf)
	//if err != nil {
	//	fmt.Fprintf(os.Stderr, "Cannot read from device: "+err.Error())
	//}
	//fmt.Printf("got random: %q", buf[:n])

	connbuf := bufio.NewReader(serial_conn)
	for {
		str, err := connbuf.ReadString('\r')
		if err != nil {
			fmt.Fprintln(os.Stderr, "Failed to read line from rng stream:", err.Error())
			os.Exit(1)
		}
		if len(str) > 0 {
			//fmt.Println(str)
			rng_line := strings.Split(str, ":")
			if len(rng_line) != 3 {
				fmt.Fprintln(os.Stderr, "Received rng message w/ invalid field count:", len(rng_line))
				continue
			}
			random_number, err := hex.DecodeString(strings.TrimSuffix(rng_line[1], "\n\r"))
			if err != nil {
				fmt.Fprintln(os.Stderr, "cannot convert random number from string:", rng_line[1], ";-(")
				continue
			}
			//fmt.Println(random_number)
			_, err = f.Write(random_number)
			if err != nil {
				fmt.Fprintln(os.Stderr, "cannot write random number to file.")
			}
			//fmt.Println("Wrote", num_bytes)
		}
	}
}
