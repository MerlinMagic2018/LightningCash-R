
# LightningCashr Core integration/staging tree
===========================================
LightningCashr Core new release version : 1.0.0

README.md Updated on September 22 2023

What is LightningCashr ?
----------------------

LightningCashr is a coin based on Lightning-Cash ( LNCR ),with Yespower POW algorithm, including DarkGravityWave ( difficulty changes for every block ) and possible CURRENT block difficulty change over time to protect against high network hash variation and long stale tip. It is also 15 times faster than LitecoinCash. For full details, please visit our website at https://coin.lightningcashr.org

Compiled binary releases are available here on Github.
Whitepapers will be available soon.

Current supported version are 1.1.0

LightningCashr Core is the full node software that makes up the backbone of the LightningCashr p2p network.


Website : https://coin.lightningcashr.org

Pool : https://pool.lightningcashr.org

Block Explorer : https://explorer.lightningcashr.org

Discord Server : https://discord.gg/


Exchanges : 
 


Integration under way : 



Any OS build instructions :

https://github.com/diabaths/LightningCashr-Coin/blob/master/doc/build-ALL-OS.md




LightningCashr Characteristics
---------------------------------------------------------------------------


- Hash Algorithm : Yespower POW algorithm    ( CPU only )

( With the addition of modified Dash's DarkGravityWave v3 algorithm, that changes difficulty every block, and progressively lowers difficulty if long stale tips happens. Tested and fully working !!) ---> check DarkGravity V3 (https://github.com/dashpay/dash/blob/master/src/pow.cpp#L82) (By Evan Duffield)
 <evan@dash.org>

And modifications from LighningCashDev made here :        https://github.com/diabaths/LightningCashr-Coin/blob/master/src/pow.cpp#L110

( Difficulty adjusted every block and possibly in CURRENT block if a long network stale tip happens !! This protects LightningCashr from high hash variations and " high hash attacks " , that happens frequently for new coins and kills them by making the mining difficulty too high for the " normal " network's hash rate. )


- The Hive : A "hodl and profit" mining system accessible to everyone, that works side by side with POW mining, invented by LitecoinCash's developers !!

- YespowerLTNCG / Hive blocks ratio : 50 % / 50 % ( active since Hive 1.1 )

- Official Mining Pool http://pool.lightningcashr.org

- 10 seconds block time. ( In chainparams.cpp. is set at 20 seconds, with hive, makes it 10s)

- Number of confirmations needed for a transaction : 6  (transaction is confirmed in approximately 1 minute)

- Block Reward : 1.25 LightningCashr

- Halved every 8.400.000 blocks


The Hive Characteristics
---------------------------------------------------------------------------

- blocks for a new bee to mature: 48 * 24 = 1152 Blocks or ~= 3 Hours 12 minutes

- blocks a bee lives for after maturation : 48 * 24 * 21= 24192 Blocks or ~= 2 Days 19 hours

- Target Hive block frequency (1 out of this many blocks should be Hivemined) : 1 Every 2 Blocks 

- Hive 1.1- Activated: June 17, 2023 (more bee lives and more) 

- OPTIONAL to return 10% of bee Creation to community fund( will by used to promote the coin)
	



License
-------

LightningCashr Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

A copy of this license can be found here :

https://github.com/diabaths/LightningCashr-Coin/blob/master/COPYING


Development Process
-------------------

You can fork LightningCashr github's repository, tweak the code and suggest pull request if you have bug fixes or improvements to propose.

Testing
-------

For now, testing is done privately before releasing new wallet's versions.

Translations
------------

Any translation corrections or expansions are welcomed as GitHub pull requests.
