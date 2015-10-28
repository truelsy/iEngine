#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import time
import select
import socket
import struct
import ChatMessage_pb2

CMD_AUTH = 0x0100
CMD_CHAT = 0x0200

def sendPacket(cmd, sock, msg) :
	m = msg.SerializeToString()

	packet = struct.pack("<BHBH", 0xff, len(m), 0x01, cmd)
	packet = packet + m

	sock.sendall(packet)

def read(sock, size) :
	buf = ""
	while (size > 0) :
		data = sock.recv(size)
		buf += data;
		size -= len(data)
	return buf

def printMessage(sock) :
	header = read(sock, 6)
	dummy, bodySize, flag, command = struct.unpack("<BHBH", header)

	body = read(sock, bodySize)
	ack = ChatMessage_pb2.ChatMessage()
	ack.ParseFromString(body)

	sys.stdout.write("\n" + ack.msg + "\n")
	sys.stdout.flush()


def main() :
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(('104.155.213.172', 54486))

	# Auth
	sys.stdout.write("Input your name : ")
	sys.stdout.flush()
	name = sys.stdin.readline().strip()

	auth = ChatMessage_pb2.AuthRequest()
	auth.nickname = name
	sendPacket(CMD_AUTH, sock, auth)

	term = False

	while (not term) :
		sys.stdout.write("%s > " % name)
		sys.stdout.flush()

		inputReady, outputReady, exceptReady = select.select([sys.stdin, sock], [], [])

		for input in inputReady :
			if (input == sys.stdin) :
				line = sys.stdin.readline().strip()
				if (not line) : continue

				line = line.strip()
				if (line.lower() == "bye") :
					term = True
					break

				chat = ChatMessage_pb2.ChatMessage()
				chat.msg = unicode(line, 'utf-8')
				sendPacket(CMD_CHAT, sock, chat)
			elif (input == sock) :
				printMessage(sock)

	try : sock.close()
	except : pass


if __name__ == "__main__" : main()
