<img src="https://i.ibb.co/4ZK5Q9Rt/bitcoin256.png" alt="bitcoin256" border="0"></a><br /><a target='_blank' href='https://imgbb.com/'></a><br />

LightningCash Reborn !!
LNCR

# LightningCashr Core integration/staging tree
===========================================
LightningCashr Core new release version : 2.0.2

README.md Updated on July 14th 2025

What is LightningCashr ?
----------------------

LightningCashr is a coin based on Lightning-Cash ( LNC ),with Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. It is also 15 times faster than LitecoinCash. For full details, please visit our website at https://lightningcash-reborn.space or 
https://lightningcashr.org

A snapshot of the old LightningCash's chain has been made on September 20th 2023, and all old balances can be claimed in the new LightningCash-Reborn wallet !!
( except addresses with less than 100 LNC )


Compiled binary releases are available here on Github.
Whitepapers will be available soon.

Current supported versions : 2.0.0 - 2.0.1 - 2.0.2

LightningCashr Core is the full node software that makes up the backbone of the LightningCashr p2p network.


Website : https://lightningcash-reborn.space

Alternative website : https://lightningcashr.org

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

- Total possible supply : 91 000 000 coins

- Extensive total supply by halving period : see ----> https://github.com/MerlinMagic2018/LightningCash-R/supply.md

The Hive Characteristics
---------------------------------------------------------------------------

- blocks for a new bee to mature: 48 * 24 = 1152 Blocks or ~= 3 Hours 21 minutes 36 seconds

- blocks a bee lives for after maturation : 48 * 24 * 14 = 16128 Blocks or ~= 1 day 23 hours 2 minutes 24 seconds

- Target Hive block frequency (1 out of this many blocks should be Hivemined) : 1 Every 2 Blocks 

- 50% of bee Creation fee goes to the community fund, and those funds will be used to promote the coin and list it on new exchanges
	



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
