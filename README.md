#teflon - Teflon Don in bot form. 

####requires libcurl and libxml2

BUILD: 'make'

USAGE: 
`./teflon [#][#]<server>:<port> <nick> <channel>`

`(#<server> for SSL.)`

`(##<server> for no-verify SSL.)`

Depending on your shell configuration,
you may need to escape the '#' characters.

Sample invocation:
`./teflon ##irc.myserver.net:6697 teflon #mychannel`

teflon supports the following commands:

.stump \<nick\> --- generates an Trumpean insult that will instantly put \<nick\> on suicide watch.

.deport \<nick\> \<reason\> --- alias for kick command.
