/**
 * rijndael-alg-fst.h
 *
 * @version 3.0 (December 2000)
 *
 * Optimised ANSI C code for the Rijndael cipher (now AES)
 *
 * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
 * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
 * @author Paulo Barreto <paulo.barreto@terra.com.br>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __RIJNDAEL_ALG_FST_H
#define __RIJNDAEL_ALG_FST_H

	#define MAXKC	(256/32)
	#define MAXKB	(256/8)
	#define MAXNR	14


	int rijndaelKeySetupEnc(unsigned long rk[/*4*(Nr + 1)*/], const unsigned char cipherKey[], int keyBits);
	#ifdef DECRYPT
		int rijndaelKeySetupDec(unsigned long rk[/*4*(Nr + 1)*/], const unsigned char cipherKey[], int keyBits);
	#endif
	void rijndaelEncrypt(const unsigned long rk[/*4*(Nr + 1)*/], int Nr, const unsigned char pt[16], unsigned char ct[16]);
	#ifdef DECRYPT
		void rijndaelDecrypt(const unsigned long rk[/*4*(Nr + 1)*/], int Nr, const unsigned char ct[16], unsigned char pt[16]);
	#endif
	#ifdef INTERMEDIATE_VALUE_KAT
		void rijndaelEncryptRound(const unsigned long rk[/*4*(Nr + 1)*/], int Nr, unsigned char block[16], int rounds);
		void rijndaelDecryptRound(const unsigned long rk[/*4*(Nr + 1)*/], int Nr, unsigned char block[16], int rounds);
	#endif /* INTERMEDIATE_VALUE_KAT */

#endif /* __RIJNDAEL_ALG_FST_H */

