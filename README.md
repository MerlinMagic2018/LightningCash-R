LightningCash Reborn !!
LNCR

# LightningCashr Core integration/staging tree
===========================================
LightningCashr Core new release version : 2.0.0

MANDATORY WALLET UPGRADE !!! You have until January 1st 2025 to upgrade your wallet.

README.md Updated on December 14th 2024

What is LightningCashr ?
----------------------

LightningCashr is a coin based on Lightning-Cash ( LNC ),with Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. It is also 15 times faster than LitecoinCash. For full details, please visit our website at https://lightningcashr.org

A snapshot of the old LightningCash's chain has been made on September 20th 2023, and all old balances can be claimed in the new LightningCash-Reborn wallet !!
( except addresses with less than 100 LNC )


Compiled binary releases are available here on Github.
Whitepapers will be available soon.

Current supported version : 2.0.0

LightningCashr Core is the full node software that makes up the backbone of the LightningCashr p2p network.


Website : https://lightningcash-reborn.space

Pool : https://pool.lightningcash-reborn.space

Block Explorer : https://explorer.lightningcash-reborn.space



Any OS build instructions :

https://github.com/MerlinMagic2018/LightningCash-R/blob/master/doc/build-ALL-OS.md




LightningCashr Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : Yespower POW algorithm    ( CPU only )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!) ---> check DarkGravity V3 (https://github.com/dashpay/dash/blob/master/src/pow.cpp#L82) (By Evan Duffield)
 <evan@dash.org>

And modifications from LighningCashDev made here :        https://github.com/MerlinMagic2018/LightningCash-R/blob/master/src/pow.cpp#L110

( Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCashr from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. )


- The Hive : A "hodl and profit" mining system accessible to everyone, that works side by side with POW mining, invented by LitecoinCash's developers !!

- POW mining algorithm : YespowerLTNCG ( cpu only algo ) / Hive blocks ratio : 50 % / 50 % ( active since Hive 1.1 )

- Official Mining Pool : https://pool.lightningcash-reborn.space

- Official block explorer : https://explorer.lightningcash-reborn.space

- 10 seconds block time. ( In chainparams.cpp. is set at 20 seconds, with hive, makes it 10s, the real average block time is exactly 10.5 seconds)

- Number of confirmations needed for a transaction : 6  (transaction is confirmed in approximately 1 minute)

- Block Reward : 1.25 LightningCashr

- Halved every 8.400.000 blocks ( +- 2 years 9 months and 17 days )

- Extensive total supply by halving period :

		- block 1 - 7 			: 70 000 000 	LNCR , which 66 414 845 LNCR has been used for the old LightningCash ( LNC ) balances claims, and 3 585 155 as a new premine.
		  				  at block 48 725 , 1 682 694 coins were sent to an old exchange's admin to cover the old LNC to LNCR swap. So the true available dev premine was
						  1 902 461 LNCR.

See in explorer : block 238, 241, 248, 254, 265, 270, 272, 276, 280, 283, 286, 289, 291, 329, 344, 345, 352, 356, 360, 367, 673, and finaly 48725.


To this day, on December 14th 2024, at block height 3 707 245,     
74 634 033.75 coins have been emitted so far, 
1 766 134.75 coins have been burned in the hive, 
so 72 867 899 is the actual total supply. 
38.11 % of emitted coins have been burned. ( 1 766 134 / 4 634 033 LNCR)

It is therefore estimated that at this rate, the 21 000 000 mineable coins will all be mined on October 21st 2088, and out of those 21 000 000, around 8 003 562 coins will have been burned.
So it is estimated that when all coins will be mined, in October 2088, the total supply will be 82 996 438 LNCR.


1.25 reward	- block 8 - 8 399 999 		: 10 500 000	LNCR    September 23rd 2023 - July 10th 2026		TOTAL 80 500 000 / 91 000 000	CORRECTED TOTAL  76 498 219 / 82 996 438																														
0.625 reward	- block 8 400 000 - 16 799 999 	:  5 250 000 	LNCR	July 10th 2026      - April 26th 2029		TOTAL 85 750 000 / 91 000 000	CORRECTED TOTAL  79 747 329 / 82 996 438
0.3125 reward	- block 16 800 000 - 25 199 999 :  2 625 000 	LNCR	April 26th 2029     - February 10th 2032	TOTAL 88 375 000 / 91 000 000	CORRECTED TOTAL  81 371 883 / 82 996 438
0.15625	reward	- block 25 200 000 - 33 599 999 :  1 312 500 	LNCR	February 10th 2032  - November 27th 2034	TOTAL 89 687 500 / 91 000 000	CORRECTED TOTAL  82 184 161 / 82 996 438
0.078125 reward - block 33 600 000 - 41 999 999 :    656 250 	LNCR	November 27th 2034  - September 13th 2037	TOTAL 90 343 750 / 91 000 000	CORRECTED TOTAL  82 590 299 / 82 996 438
0.0390625 reward- block 42 000 000 - 50 399 999 :    328 125 	LNCR	September 13th 2037 - June 30th 2040		TOTAL 90 671 875 / 91 000 000	CORRECTED TOTAL  82 793 369 / 82 996 438
0.01953125 reward block 50 400 000 - 58 799 999 :    164 062.5 	LNCR	June 30th 2040      - April 17th 2043		TOTAL 90 835 938 / 91 000 000	CORRECTED TOTAL  82 894 904 / 82 996 438
...
...
Febrary 1st 2046 ... November 17th 2048 ... September 4th 2051
 ... June 21st 2054 ... April 7th 2057 --- January 23rd 2060 ( 12th halving )
...  October 21st 2088 ( 26th and last possible halving )

Maximum supply ( maximum theoretical possible supply, reached in October 2088, but will always be less due to coins being burned in bee creation transactions... )

	91 000 000 coins	( 70 000 000 divided as follows : 66 414 845 coins for the old LNC balance claims, 1 682 694 coins for an old exchange swap, and 1 902 461 as a new premine )
				( 21 000 000 mineable coins )

Approximately 38.11 % of coins will be burned.......

To push it even further.... since the percentage of the bee creation fee that will be burned will go down from 90 or 100 % to 50 % on January 1st 2025, blockheight 3 864 378, it is estimated
that from this blockheight, only 20 % of the emitted coins will end up being burned in the hive.

XXXXX ---- So, final most accurate coin supply prediction : ---- XXXXX 	
	
	January 1st 2025   :  72 989 468 / 85 925 089     84.95 % of Max supply reached, 18.77 % of mineable coins mined.
	July 10th 2026     :  77 525 089 / 85 925 089     90.22 % of Max supply reached, 47.25 % of mineable coins mined.
	April 26th 2029    :  81 725 089 / 85 925 089	  95.11 % of Max supply reached, 73.63 % of mineable coins mined.
	February 10th 2032 :  83 825 089 / 85 925 089	  97.56 % of Max supply reached, 86.81 % of mineable coins mined.
	November 27th 2034 :  84 875 089 / 85 925 089	  98.78 % of Max supply reached, 93.41 % of mineable coins mined.
	     ...
	October 21st 2088  :  85 925 089 / 85 925 089	  100 %   of Max supply reached, 100 %   of mineable coins mined.



The Hive Characteristics
---------------------------------------------------------------------------

- blocks for a new bee to mature: 48 * 24 = 1152 Blocks or ~= 3 Hours 21 minutes 36 seconds

- blocks a bee lives for after maturation : 48 * 24 * 14 = 16128 Blocks or ~= 1 day 23 hours 2 minutes 24 seconds

- Target Hive block frequency (1 out of this many blocks should be Hivemined) : 1 Every 2 Blocks 

- 10% of bee Creation fee goes to the community fund														
( it will be 50 % after January 1st 2025, and those funds will be used to promote the coin and list it on new exchanges )
	



License
-------

LightningCashr Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

A copy of this license can be found here :

https://github.com/MerlinMagic2018/LightningCash-R/blob/master/COPYING


Development Process
-------------------

You can fork LightningCashr github's repository, tweak the code and suggest pull request if you have bug fixes or improvements to propose.

Testing
-------

For now, testing is done privately before releasing new wallet's versions.

Translations
------------

Any translation corrections or expansions are welcomed as GitHub pull requests.
