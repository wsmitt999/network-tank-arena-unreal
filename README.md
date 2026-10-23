# Network Tank Arena - Artifact Pickup

**Main Functionality**
A special pickup has been added to the map. When a player picks it up by touching it, it replicates appropriately and shows a pickup message for all users.
This pickup message shows who picked it up. If it is the local player, it displays as "You". If else, it displays as the other player's name.

**How to run**
Build the project with Unreal Engine 5.7.4. Use PIE (Play In Editor) and set Net Mode to Listen Server.

**Important Networking Decisions**
The pickup is server-authoritative to prevent basic forms of cheating. The pickup message is also server-authoritative for the same reasons and uses a Multicast to broadcast the message to every connected client.

**Network Conditions**
Tested with:
- no impairment
- high ping (500-1500ms range for both incoming and outgoing traffic)
- very high packet loss (10%)
- both

**Issue encountered on the way**
While developing this project, I discovered an issue with the pickup messages. I was still using on-screen messages in order to debug, and I noticed that the messages were appearing for both players.
Eg:
- You picked up the artifact!
- \[OtherPlayer\] picked up the artifact!
I spent a while trying to fix this issue and implemented a ton of safeguards, but then I did some more research and found out that the debug on-screen messages are shared between all simulated clients.
After implementing the pickup notification widget, it was working as intended.

**Limitations/Trade-offs/Known issues**
- Popups can overlap.
- UI is hard-coded in C++.
- Late-joiners will not see the popup if the artifact was picked up before they joined.
With a bit more work, these could definitely be solved in a good way.
