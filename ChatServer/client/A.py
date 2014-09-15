#!/usr/bin/env python

import sys
import time
import select
import socket
import struct
import ChatMessage_pb2

CMD_AUTH = 0x0100
CMD_CHAT = 0x0200

cnt = 0

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

	global cnt

	sys.stdout.write("%d : %s\n" % (cnt, ack.msg))
	sys.stdout.flush()
	cnt += 1


def main() :
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.connect(('', 54486))

	# Auth
	sys.stdout.write("Input your name : ")
	sys.stdout.flush()
	name = sys.stdin.readline().strip()

	auth = ChatMessage_pb2.AuthRequest()
	auth.nickname = name
	sendPacket(CMD_AUTH, sock, auth)

	term = False

	while (not term) :
		chat = ChatMessage_pb2.ChatMessage()
		chat.msg = "PING"
		sendPacket(CMD_CHAT, sock, chat)
		printMessage(sock)

	try : sock.close()
	except : pass


if __name__ == "__main__" : main()
