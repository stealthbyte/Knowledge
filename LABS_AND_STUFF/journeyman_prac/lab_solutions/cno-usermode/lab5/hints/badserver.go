package main

import (
	"github.com/gliderlabs/ssh"
	"io"
	"log"
)

func conhandler(s ssh.Session) {
	io.WriteString(s, "\nWelcome! This is totally the box you tried to connect to...\n\n\n")
	log.Printf("Got another victim!\n")
}

func main() {
	ssh.Handle(conhandler)
	log.Fatal(ssh.ListenAndServe(":22", nil))
}
