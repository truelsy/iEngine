# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: ChatMessage.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='ChatMessage.proto',
  package='',
  serialized_pb='\n\x11\x43hatMessage.proto\"\x1f\n\x0b\x41uthRequest\x12\x10\n\x08nickname\x18\x01 \x02(\t\"\x1a\n\x0b\x43hatMessage\x12\x0b\n\x03msg\x18\x01 \x02(\t')




_AUTHREQUEST = _descriptor.Descriptor(
  name='AuthRequest',
  full_name='AuthRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='nickname', full_name='AuthRequest.nickname', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=21,
  serialized_end=52,
)


_CHATMESSAGE = _descriptor.Descriptor(
  name='ChatMessage',
  full_name='ChatMessage',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='msg', full_name='ChatMessage.msg', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=54,
  serialized_end=80,
)

DESCRIPTOR.message_types_by_name['AuthRequest'] = _AUTHREQUEST
DESCRIPTOR.message_types_by_name['ChatMessage'] = _CHATMESSAGE

class AuthRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _AUTHREQUEST

  # @@protoc_insertion_point(class_scope:AuthRequest)

class ChatMessage(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _CHATMESSAGE

  # @@protoc_insertion_point(class_scope:ChatMessage)


# @@protoc_insertion_point(module_scope)