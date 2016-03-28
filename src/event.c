#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libircclient.h"
#include "log.h"
#include "event.h"
#include "stump.h"

void dump_event(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    char buf[512];
    int cnt;

    buf[0] = '\0';
    for (cnt = 0; cnt < count; cnt++){
        if (cnt)
            strcat(buf, "|");
        strcat(buf, params[cnt]);
    }
    addlog("Event \"%s\", origin: \"%s\", params: %d [%s]", event, 
                                            origin ? origin : "NULL", cnt, buf);
}

void event_join(irc_session_t *session, const char *event,
                const char *origin, const char **params, unsigned int count)
{
    dump_event(session, event, origin, params, count);
}

void event_notice (irc_session_t * session, const char * event, 
                const char * origin, const char ** params, unsigned int count)
{
    char buf[256];
    irc_ctx_t *ctx;
    
    dump_event(session, event, origin, params, count);

    if (!origin)
        return;
    if (strcasecmp(origin, "nickserv"))
        return;
    
    ctx = irc_get_ctx(session);
    if (!ctx->password)
        return;
    sprintf(buf, "IDENTIFY %s", ctx->password);
    irc_cmd_msg(session, "nickserv", buf);
}

void event_connect(irc_session_t *session, const char *event,
                const char *origin, const char **params, unsigned int count)
{
    irc_ctx_t *ctx = (irc_ctx_t *) irc_get_ctx (session);
    dump_event(session, event, origin, params, count);
    irc_cmd_join(session, ctx->channel, 0);
}

void event_privmsg(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    dump_event(session, event, origin, params, count);

    printf("'%s' said me (%s): %s\n", 
        origin ? origin : "someone",
        params[0], params[1]);
}

void event_kick(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    dump_event(session, event, origin, params, count);
    irc_cmd_join(session, params[0], 0);

}

void event_channel(irc_session_t *session, const char *event, 
                const char *origin, const char **params, unsigned int count)
{
    if (count != 2)
        return;

    if (!origin)
        return;
    
    if (!strncmp(params[1], ".stump", 6)) {
        stump(session, params);
    }
    if (!strncmp(params[1], ".deport", 7)) {
        deport(session, params);
    }
}

void event_numeric(irc_session_t *session, unsigned int event, 
                const char *origin, const char **params, unsigned int count)
{
    char buf[24];
    sprintf(buf, "%d", event);

    dump_event(session, buf, origin, params, count);
}
