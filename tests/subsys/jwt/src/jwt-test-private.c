/*
 * Copyright (C) 2018, Linaro, Ltd
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#if defined(CONFIG_JWT_SIGN_RSA)

/* To generate the key in the correct format use the following command:
 * $ openssl genrsa 2048 | openssl rsa -outform DER | xxd -i
 */

unsigned char jwt_test_private_der[] = {
	0x30, 0x82, 0x04, 0xa5, 0x02, 0x01, 0x00, 0x02, 0x82, 0x01, 0x01, 0x00,
	0xc3, 0x10, 0x4f, 0x09, 0xcb, 0xfa, 0xfc, 0xc6, 0xb3, 0x93, 0x38, 0x00,
	0x6b, 0x0c, 0x6a, 0x68, 0xa8, 0x65, 0x0b, 0xf3, 0x88, 0x57, 0x0f, 0x94,
	0x5c, 0xf0, 0x93, 0xb1, 0x24, 0xff, 0x3e, 0xe8, 0x6b, 0xf3, 0xbd, 0xd1,
	0xd3, 0x9e, 0x64, 0x2e, 0x65, 0xfd, 0x2f, 0x57, 0xe9, 0x8d, 0x12, 0x4b,
	0xae, 0x3f, 0xe5, 0x04, 0x77, 0xa9, 0x78, 0x1f, 0x8a, 0xe8, 0x03, 0x6c,
	0x06, 0x6f, 0x9e, 0x77, 0x5c, 0xfe, 0x87, 0x2a, 0xb3, 0xb7, 0x7f, 0xaa,
	0xe1, 0x9b, 0x2b, 0x79, 0x69, 0xac, 0x06, 0xb3, 0x2e, 0x2f, 0x0d, 0x3a,
	0x99, 0x06, 0xf4, 0xb1, 0x0a, 0x3a, 0x90, 0x75, 0x18, 0x29, 0x48, 0xc4,
	0x2b, 0xd0, 0x67, 0x41, 0x4b, 0x82, 0x47, 0xf3, 0xae, 0x6d, 0x05, 0x37,
	0xe4, 0x4d, 0x7e, 0x19, 0x61, 0x2c, 0xb1, 0x4c, 0x9e, 0x05, 0xcb, 0x98,
	0x7d, 0x1c, 0x92, 0x90, 0xff, 0xef, 0xa0, 0x5c, 0x5d, 0xee, 0xa9, 0xce,
	0x04, 0x8d, 0xbc, 0xea, 0xb0, 0x37, 0x97, 0x65, 0x95, 0xdb, 0x23, 0xb7,
	0x8a, 0x9e, 0x8e, 0x14, 0x08, 0xb1, 0x8a, 0x64, 0x85, 0xc7, 0x55, 0xe0,
	0x1e, 0x7f, 0x5a, 0x8c, 0x78, 0xc1, 0x03, 0xcb, 0x0f, 0x8c, 0x51, 0x8d,
	0x6f, 0x55, 0x0a, 0x6a, 0xc5, 0x82, 0xf4, 0xf5, 0xae, 0x1a, 0xcb, 0x5a,
	0x16, 0x45, 0x0f, 0xe2, 0xc3, 0xb8, 0xd2, 0xe1, 0x50, 0xc7, 0x02, 0xe5,
	0xf8, 0xd5, 0x63, 0xb9, 0xd4, 0xc7, 0x6b, 0xf2, 0x03, 0x25, 0xfb, 0x18,
	0x64, 0x66, 0xab, 0xbc, 0xd8, 0x04, 0xf8, 0xb9, 0x88, 0x23, 0x5e, 0x6a,
	0x27, 0x20, 0xa2, 0x7c, 0xaf, 0x75, 0xad, 0xdc, 0x38, 0xea, 0x03, 0x25,
	0x42, 0x39, 0xdd, 0x49, 0xf3, 0xaa, 0xd4, 0x46, 0x98, 0x57, 0x61, 0x1e,
	0xc8, 0x56, 0x0e, 0xfc, 0x4b, 0x9a, 0xe2, 0x27, 0xc0, 0x83, 0xc5, 0xde,
	0xb2, 0x3e, 0x7d, 0xd9, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x82, 0x01,
	0x00, 0x53, 0x34, 0x7a, 0xb2, 0x70, 0x1b, 0x10, 0x85, 0xe5, 0x85, 0x8f,
	0xa6, 0xc6, 0x02, 0xd6, 0xc4, 0xeb, 0xc7, 0xa6, 0x70, 0x6c, 0x20, 0xce,
	0xd9, 0x77, 0x47, 0x7b, 0x88, 0x5d, 0x82, 0xaf, 0x1f, 0xcf, 0x24, 0x7a,
	0x3c, 0xad, 0xaa, 0xeb, 0x8a, 0xcb, 0x20, 0x53, 0x78, 0x89, 0x7c, 0x0e,
	0x68, 0xcf, 0xdc, 0x1b, 0x61, 0x6b, 0x75, 0x93, 0x72, 0xc8, 0xfa, 0xac,
	0xb3, 0x00, 0x8f, 0xce, 0x25, 0x96, 0x3a, 0x50, 0x07, 0xcb, 0xff, 0x22,
	0xeb, 0x0e, 0x2e, 0xf5, 0x3d, 0x65, 0xf0, 0xe6, 0x2e, 0xba, 0x2d, 0x81,
	0xd4, 0x14, 0x00, 0x5e, 0x91, 0xc2, 0x0e, 0x37, 0x1e, 0x6b, 0x7e, 0xf8,
	0xb6, 0x0d, 0x06, 0x69, 0x74, 0xf5, 0xb3, 0x8d, 0xe3, 0x88, 0xec, 0xa2,
	0x04, 0x34, 0x53, 0x45, 0xcd, 0xa7, 0xa0, 0xbb, 0xc4, 0x35, 0xc7, 0xbd,
	0x0f, 0xbf, 0x96, 0x57, 0x82, 0x98, 0x59, 0x6b, 0xaa, 0x1c, 0x05, 0xc6,
	0x9a, 0x0b, 0x55, 0xf4, 0x30, 0xd5, 0xf9, 0xb7, 0xe9, 0x90, 0xff, 0x77,
	0x8c, 0x92, 0xdd, 0x42, 0x6f, 0xcd, 0x08, 0xff, 0x84, 0x59, 0xed, 0xdf,
	0x86, 0x55, 0x9c, 0x30, 0x29, 0x19, 0x55, 0xcc, 0x86, 0x10, 0xd0, 0x4b,
	0x59, 0xa4, 0x34, 0xc5, 0x75, 0x0a, 0xca, 0x41, 0xf0, 0x02, 0x97, 0xa1,
	0x2b, 0x36, 0x2c, 0xca, 0x5e, 0xca, 0xe6, 0xe7, 0xe0, 0xcb, 0xe8, 0x1d,
	0xa0, 0x09, 0x8f, 0x4d, 0xf9, 0x63, 0x97, 0xa2, 0x0a, 0xd8, 0x88, 0x1e,
	0x6e, 0xd4, 0x0c, 0x30, 0xad, 0xf2, 0xc7, 0xa7, 0xa8, 0xe2, 0xce, 0x23,
	0x3e, 0xa7, 0x02, 0x3d, 0xe3, 0xff, 0xe4, 0xf1, 0x7c, 0xdb, 0xad, 0x88,
	0x1e, 0xcd, 0xc7, 0x0e, 0x90, 0xf3, 0xe5, 0xc2, 0x88, 0x37, 0x06, 0xd6,
	0x92, 0x4f, 0x67, 0x77, 0xd6, 0x47, 0xc6, 0x59, 0x1a, 0x3a, 0xdf, 0x12,
	0x79, 0x60, 0x5e, 0x68, 0x5d, 0x02, 0x81, 0x81, 0x00, 0xe5, 0x79, 0x43,
	0xa6, 0x8a, 0x67, 0x71, 0xed, 0x3c, 0xec, 0x75, 0xa8, 0x56, 0x3d, 0x37,
	0x39, 0x3d, 0xbd, 0x71, 0x98, 0x4f, 0x6c, 0x65, 0x02, 0x5c, 0xf6, 0x1f,
	0xf1, 0x8c, 0xe5, 0x92, 0xd6, 0xe0, 0x60, 0x23, 0x3d, 0x90, 0x07, 0x9c,
	0x4c, 0xa6, 0x1d, 0x6f, 0x94, 0x2a, 0x7c, 0x71, 0x40, 0xf3, 0x48, 0x89,
	0xb3, 0xe0, 0x7f, 0x28, 0xf9, 0xc4, 0x57, 0xbf, 0x29, 0x9c, 0x74, 0xf1,
	0x24, 0x31, 0xdb, 0xd3, 0x8b, 0x04, 0xfa, 0x1f, 0x2d, 0x37, 0x12, 0x96,
	0xe2, 0x07, 0xd1, 0x6b, 0x2f, 0x0b, 0xc3, 0x52, 0x51, 0x23, 0x0a, 0x71,
	0x5f, 0x44, 0xde, 0x93, 0x60, 0x91, 0x9b, 0x9a, 0x83, 0x0a, 0xc0, 0xd1,
	0xaa, 0xbc, 0xc9, 0xab, 0x66, 0xb4, 0x20, 0x7c, 0x72, 0x17, 0x94, 0x9c,
	0x3d, 0xd0, 0xc9, 0x08, 0x35, 0xfe, 0xb0, 0x07, 0x16, 0xf0, 0x98, 0xc7,
	0x22, 0x6e, 0x83, 0xf4, 0x8f, 0x02, 0x81, 0x81, 0x00, 0xd9, 0x9c, 0xc3,
	0x01, 0x27, 0x3c, 0xa6, 0xa1, 0xbe, 0xb1, 0x1e, 0xe8, 0x2e, 0x16, 0xc7,
	0xcc, 0x70, 0x27, 0xe5, 0x84, 0xdb, 0xd0, 0x76, 0xdd, 0x45, 0x10, 0xf8,
	0x80, 0x41, 0x60, 0x24, 0xfa, 0x04, 0x1e, 0x44, 0x29, 0x31, 0x2e, 0x63,
	0x20, 0xe7, 0x44, 0xc2, 0x19, 0x39, 0x83, 0x49, 0xf7, 0x0b, 0x48, 0x76,
	0xf0, 0x4a, 0xab, 0xad, 0xc9, 0xfc, 0xcf, 0x06, 0x5e, 0x5b, 0x1b, 0xb9,
	0x33, 0xa8, 0xbe, 0x19, 0x23, 0x1f, 0x18, 0xe1, 0x74, 0xb5, 0x7b, 0xfd,
	0x56, 0x2e, 0x08, 0xee, 0xe2, 0xa8, 0x2b, 0x8a, 0x24, 0x7f, 0x13, 0xc2,
	0xb7, 0xd7, 0x32, 0x58, 0x75, 0xb5, 0x06, 0x8f, 0x40, 0x8d, 0xd8, 0x47,
	0x5f, 0x0c, 0xa9, 0x08, 0x65, 0x06, 0x6d, 0x5f, 0xad, 0xa9, 0x1b, 0xcd,
	0x7c, 0x4c, 0x49, 0xb7, 0x51, 0x0f, 0x40, 0xf2, 0x7c, 0x46, 0xcc, 0x7f,
	0x46, 0xb3, 0x78, 0xab, 0x17, 0x02, 0x81, 0x81, 0x00, 0x9d, 0xaa, 0x8d,
	0x0d, 0xcf, 0x88, 0xb5, 0xe1, 0x5f, 0xfb, 0xc3, 0xf7, 0x8e, 0x91, 0x6e,
	0xf1, 0x18, 0x77, 0xa2, 0xa6, 0x18, 0xe4, 0x0c, 0x1e, 0xec, 0x8a, 0x7b,
	0x76, 0x36, 0xed, 0xc1, 0xcf, 0x7b, 0xc8, 0xf2, 0x4d, 0xa9, 0x39, 0x61,
	0x6d, 0xd4, 0xb0, 0xe2, 0xea, 0x35, 0xaa, 0xdd, 0x11, 0xd8, 0x56, 0x5d,
	0xae, 0xc6, 0xaf, 0xf8, 0x12, 0x6c, 0x92, 0x31, 0x99, 0x40, 0x93, 0xa4,
	0xdf, 0xce, 0x96, 0x10, 0xaa, 0x33, 0x18, 0x3b, 0xf5, 0x90, 0x22, 0x68,
	0xa3, 0x12, 0xe6, 0x13, 0xb3, 0xf6, 0x6c, 0x14, 0x92, 0x04, 0xf6, 0x76,
	0xe1, 0x13, 0x6b, 0xae, 0x6d, 0x2a, 0x7e, 0x74, 0x52, 0x57, 0x40, 0x16,
	0x8f, 0x2e, 0x39, 0x8e, 0x57, 0xf9, 0xa9, 0x5c, 0x77, 0xf3, 0xb5, 0xbb,
	0x08, 0x49, 0xfc, 0xe6, 0x74, 0x5a, 0xf2, 0xcd, 0x5c, 0x38, 0xa1, 0x70,
	0x29, 0x3b, 0x2f, 0xd5, 0x07, 0x02, 0x81, 0x81, 0x00, 0xa9, 0xe0, 0xa7,
	0x96, 0x89, 0xe3, 0xde, 0x0d, 0x1c, 0x49, 0x36, 0xe0, 0x77, 0xc2, 0x4e,
	0xdc, 0xf8, 0x9d, 0x1c, 0x9e, 0xb7, 0xde, 0xa8, 0x3c, 0x1a, 0x03, 0xa2,
	0x8a, 0xb4, 0xfe, 0x72, 0x3f, 0x3e, 0xe1, 0xc5, 0xc2, 0x06, 0x63, 0xb8,
	0xfc, 0xe1, 0xad, 0x9a, 0x7c, 0x88, 0x9a, 0x46, 0x2e, 0x75, 0xff, 0x6e,
	0xe2, 0x2c, 0xd8, 0x92, 0xc5, 0xc0, 0xf6, 0x78, 0x79, 0x46, 0x54, 0x81,
	0x22, 0xa3, 0xd3, 0x2a, 0xda, 0x71, 0x17, 0x42, 0x41, 0x92, 0xbf, 0xa5,
	0x48, 0x16, 0x8d, 0xe0, 0xb6, 0xce, 0x23, 0x97, 0x26, 0xe0, 0x91, 0x33,
	0x8b, 0xc2, 0x91, 0xc9, 0xde, 0xb1, 0x9f, 0xda, 0x4a, 0xc1, 0xb0, 0x9a,
	0x1e, 0xc8, 0x4a, 0x58, 0x84, 0x4d, 0x1d, 0x2a, 0xf8, 0xed, 0xb0, 0x07,
	0x9c, 0xc6, 0x0d, 0x31, 0xef, 0x34, 0xa2, 0x35, 0xe9, 0xf0, 0x77, 0x09,
	0xd0, 0xce, 0xa0, 0x1f, 0xa7, 0x02, 0x81, 0x81, 0x00, 0xb0, 0x9d, 0xd2,
	0x7f, 0xff, 0xe0, 0x86, 0x3e, 0x5f, 0xfe, 0x83, 0xf6, 0x51, 0xa3, 0x09,
	0xb2, 0xb6, 0x4a, 0xe0, 0xdf, 0xd9, 0xdd, 0x2a, 0x9f, 0x68, 0x4c, 0xb2,
	0x4f, 0x12, 0x1d, 0x07, 0xb8, 0x75, 0x70, 0x28, 0x7a, 0x49, 0xaa, 0x56,
	0x53, 0x7a, 0xc7, 0xbf, 0xbd, 0xea, 0x38, 0x2c, 0xf0, 0x91, 0xf6, 0x71,
	0x1e, 0x97, 0x10, 0x20, 0x79, 0x2b, 0x24, 0x6c, 0x1c, 0x7b, 0x2e, 0x96,
	0x8c, 0x72, 0x13, 0xc1, 0x60, 0x7c, 0x41, 0x20, 0x8b, 0x36, 0x4c, 0xe2,
	0x65, 0x8d, 0xc5, 0x0e, 0x1e, 0x34, 0x01, 0x6d, 0x45, 0x48, 0x86, 0x6e,
	0x45, 0x43, 0xf4, 0xff, 0xb8, 0xa0, 0xa8, 0x07, 0x49, 0x54, 0x92, 0x0f,
	0x53, 0xe9, 0x2c, 0xd3, 0xdb, 0x6a, 0x8a, 0xeb, 0xc4, 0x15, 0x51, 0xdf,
	0x9c, 0x1a, 0x1a, 0x20, 0x8a, 0xed, 0xfe, 0xef, 0xe8, 0xe1, 0x04, 0x14,
	0x05, 0xfd, 0x71, 0xb0, 0x3e
};

#else /* CONFIG_JWT_SIGN_ECDSA */

/* Here's how to generate the key in the correct format:
 * - generate the key using OpenSSL:
 *   $ openssl ecparam -name prime256v1 -genkey | openssl ec -outform der | xxd -ps
 * - copy the content to https://lapo.it/asn1js/
 * - get the first OCTET STRING, which is the private key, and copy it here
 */

unsigned char jwt_test_private_der[] = {
	0xAA, 0xE2, 0xA4, 0x5F, 0x3B, 0xC4, 0xCB, 0xA5, 0x0B, 0x17, 0xAE, 0x55,
	0x4D, 0x21, 0x1B, 0xB0, 0xCC, 0x71, 0x42, 0x74, 0xBF, 0x1D, 0xC6, 0x5B,
	0xF9, 0x50, 0x93, 0x06, 0x7B, 0xCE, 0x9D, 0x75
};

#endif

unsigned int jwt_test_private_der_len = sizeof(jwt_test_private_der);
