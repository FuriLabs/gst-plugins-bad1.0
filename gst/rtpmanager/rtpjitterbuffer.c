/* GStreamer
 * Copyright (C) <2007> Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include <string.h>
#include <stdlib.h>

#include <gst/rtp/gstrtpbuffer.h>
#include <gst/rtp/gstrtcpbuffer.h>

#include "rtpjitterbuffer.h"

GST_DEBUG_CATEGORY_STATIC (rtp_jitter_buffer_debug);
#define GST_CAT_DEFAULT rtp_jitter_buffer_debug

#define MAX_WINDOW	RTP_JITTER_BUFFER_MAX_WINDOW
#define MAX_TIME	(2 * GST_SECOND)

/* signals and args */
enum
{
  LAST_SIGNAL
};

enum
{
  PROP_0
};

/* GObject vmethods */
static void rtp_jitter_buffer_finalize (GObject * object);

GType
rtp_jitter_buffer_mode_get_type (void)
{
  static GType jitter_buffer_mode_type = 0;
  static const GEnumValue jitter_buffer_modes[] = {
    {RTP_JITTER_BUFFER_MODE_NONE, "Only use RTP timestamps", "none"},
    {RTP_JITTER_BUFFER_MODE_SLAVE, "Slave receiver to sender clock", "slave"},
    {RTP_JITTER_BUFFER_MODE_BUFFER, "Do low/high watermark buffering",
        "buffer"},
    {RTP_JITTER_BUFFER_MODE_SYNCED, "Synchronized sender and receiver clocks",
        "synced"},
    {0, NULL, NULL},
  };

  if (!jitter_buffer_mode_type) {
    jitter_buffer_mode_type =
        g_enum_register_static ("RTPJitterBufferMode", jitter_buffer_modes);
  }
  return jitter_buffer_mode_type;
}

/* static guint rtp_jitter_buffer_signals[LAST_SIGNAL] = { 0 }; */

G_DEFINE_TYPE (RTPJitterBuffer, rtp_jitter_buffer, G_TYPE_OBJECT);

static void
rtp_jitter_buffer_class_init (RTPJitterBufferClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;

  gobject_class->finalize = rtp_jitter_buffer_finalize;

  GST_DEBUG_CATEGORY_INIT (rtp_jitter_buffer_debug, "rtpjitterbuffer", 0,
      "RTP Jitter Buffer");
}

static void
rtp_jitter_buffer_init (RTPJitterBuffer * jbuf)
{
  jbuf->packets = g_queue_new ();
  jbuf->mode = RTP_JITTER_BUFFER_MODE_SLAVE;

  rtp_jitter_buffer_reset_skew (jbuf);
}

static void
rtp_jitter_buffer_finalize (GObject * object)
{
  RTPJitterBuffer *jbuf;

  jbuf = RTP_JITTER_BUFFER_CAST (object);

  g_queue_free (jbuf->packets);

  G_OBJECT_CLASS (rtp_jitter_buffer_parent_class)->finalize (object);
}

/**
 * rtp_jitter_buffer_new:
 *
 * Create an #RTPJitterBuffer.
 *
 * Returns: a new #RTPJitterBuffer. Use g_object_unref() after usage.
 */
RTPJitterBuffer *
rtp_jitter_buffer_new (void)
{
  RTPJitterBuffer *jbuf;

  jbuf = g_object_new (RTP_TYPE_JITTER_BUFFER, NULL);

  return jbuf;
}

/**
 * rtp_jitter_buffer_get_mode:
 * @jbuf: an #RTPJitterBuffer
 *
 * Get the current jitterbuffer mode.
 *
 * Returns: the current jitterbuffer mode.
 */
RTPJitterBufferMode
rtp_jitter_buffer_get_mode (RTPJitterBuffer * jbuf)
{
  return jbuf->mode;
}

/**
 * rtp_jitter_buffer_set_mode:
 * @jbuf: an #RTPJitterBuffer
 * @mode: a #RTPJitterBufferMode
 *
 * Set the buffering and clock slaving algorithm used in the @jbuf.
 */
void
rtp_jitter_buffer_set_mode (RTPJitterBuffer * jbuf, RTPJitterBufferMode mode)
{
  jbuf->mode = mode;
}

GstClockTime
rtp_jitter_buffer_get_delay (RTPJitterBuffer * jbuf)
{
  return jbuf->delay;
}

void
rtp_jitter_buffer_set_delay (RTPJitterBuffer * jbuf, GstClockTime delay)
{
  jbuf->delay = delay;
  jbuf->low_level = (delay * 15) / 100;
  /* the high level is at 90% in order to release packets before we fill up the
   * buffer up to the latency */
  jbuf->high_level = (delay * 90) / 100;

  GST_DEBUG ("delay %" GST_TIME_FORMAT ", min %" GST_TIME_FORMAT ", max %"
      GST_TIME_FORMAT, GST_TIME_ARGS (jbuf->delay),
      GST_TIME_ARGS (jbuf->low_level), GST_TIME_ARGS (jbuf->high_level));
}

/**
 * rtp_jitter_buffer_set_clock_rate:
 * @jbuf: an #RTPJitterBuffer
 * @clock_rate: the new clock rate
 *
 * Set the clock rate in the jitterbuffer.
 */
void
rtp_jitter_buffer_set_clock_rate (RTPJitterBuffer * jbuf, guint32 clock_rate)
{
  if (jbuf->clock_rate != clock_rate) {
    GST_DEBUG ("Clock rate changed from %" G_GUINT32_FORMAT " to %"
        G_GUINT32_FORMAT, jbuf->clock_rate, clock_rate);
    jbuf->clock_rate = clock_rate;
    rtp_jitter_buffer_reset_skew (jbuf);
  }
}

/**
 * rtp_jitter_buffer_get_clock_rate:
 * @jbuf: an #RTPJitterBuffer
 *
 * Get the currently configure clock rate in @jbuf.
 *
 * Returns: the current clock-rate
 */
guint32
rtp_jitter_buffer_get_clock_rate (RTPJitterBuffer * jbuf)
{
  return jbuf->clock_rate;
}

/**
 * rtp_jitter_buffer_reset_skew:
 * @jbuf: an #RTPJitterBuffer
 *
 * Reset the skew calculations in @jbuf.
 */
void
rtp_jitter_buffer_reset_skew (RTPJitterBuffer * jbuf)
{
  jbuf->base_time = -1;
  jbuf->base_rtptime = -1;
  jbuf->base_extrtp = -1;
  jbuf->ext_rtptime = -1;
  jbuf->last_rtptime = -1;
  jbuf->window_pos = 0;
  jbuf->window_filling = TRUE;
  jbuf->window_min = 0;
  jbuf->skew = 0;
  jbuf->prev_send_diff = -1;
  jbuf->prev_out_time = -1;
  jbuf->need_resync = TRUE;
  GST_DEBUG ("reset skew correction");
}

/**
 * rtp_jitter_buffer_disable_buffering:
 * @jbuf: an #RTPJitterBuffer
 * @disabled: the new state
 *
 * Enable or disable buffering on @jbuf.
 */
void
rtp_jitter_buffer_disable_buffering (RTPJitterBuffer * jbuf, gboolean disabled)
{
  jbuf->buffering_disabled = disabled;
}

static void
rtp_jitter_buffer_resync (RTPJitterBuffer * jbuf, GstClockTime time,
    GstClockTime gstrtptime, guint64 ext_rtptime, gboolean reset_skew)
{
  jbuf->base_time = time;
  jbuf->base_rtptime = gstrtptime;
  jbuf->base_extrtp = ext_rtptime;
  jbuf->prev_out_time = -1;
  jbuf->prev_send_diff = -1;
  if (reset_skew) {
    jbuf->window_filling = TRUE;
    jbuf->window_pos = 0;
    jbuf->window_min = 0;
    jbuf->window_size = 0;
    jbuf->skew = 0;
  }
  jbuf->need_resync = FALSE;
}

static guint64
get_buffer_level (RTPJitterBuffer * jbuf)
{
  RTPJitterBufferItem *high_buf = NULL, *low_buf = NULL;
  guint64 level;

  /* first buffer with timestamp */
  high_buf = (RTPJitterBufferItem *) g_queue_peek_tail_link (jbuf->packets);
  while (high_buf) {
    if (high_buf->dts != -1 || high_buf->pts != -1)
      break;

    high_buf = (RTPJitterBufferItem *) g_list_previous (high_buf);
  }

  low_buf = (RTPJitterBufferItem *) g_queue_peek_head_link (jbuf->packets);
  while (low_buf) {
    if (low_buf->dts != -1 || low_buf->pts != -1)
      break;

    low_buf = (RTPJitterBufferItem *) g_list_next (low_buf);
  }

  if (!high_buf || !low_buf || high_buf == low_buf) {
    level = 0;
  } else {
    guint64 high_ts, low_ts;

    high_ts = high_buf->dts != -1 ? high_buf->dts : high_buf->pts;
    low_ts = low_buf->dts != -1 ? low_buf->dts : low_buf->pts;

    if (high_ts > low_ts)
      level = high_ts - low_ts;
    else
      level = 0;

    GST_LOG_OBJECT (jbuf,
        "low %" GST_TIME_FORMAT " high %" GST_TIME_FORMAT " level %"
        G_GUINT64_FORMAT, GST_TIME_ARGS (low_ts), GST_TIME_ARGS (high_ts),
        level);
  }
  return level;
}

static void
update_buffer_level (RTPJitterBuffer * jbuf, gint * percent)
{
  gboolean post = FALSE;
  guint64 level;

  level = get_buffer_level (jbuf);
  GST_DEBUG ("buffer level %" GST_TIME_FORMAT, GST_TIME_ARGS (level));

  if (jbuf->buffering_disabled) {
    GST_DEBUG ("buffering is disabled");
    level = jbuf->high_level;
  }

  if (jbuf->buffering) {
    post = TRUE;
    if (level >= jbuf->high_level) {
      GST_DEBUG ("buffering finished");
      jbuf->buffering = FALSE;
    }
  } else {
    if (level < jbuf->low_level) {
      GST_DEBUG ("buffering started");
      jbuf->buffering = TRUE;
      post = TRUE;
    }
  }
  if (post) {
    gint perc;

    if (jbuf->buffering && (jbuf->high_level != 0)) {
      perc = (level * 100 / jbuf->high_level);
      perc = MIN (perc, 100);
    } else {
      perc = 100;
    }

    if (percent)
      *percent = perc;

    GST_DEBUG ("buffering %d", perc);
  }
}

/* For the clock skew we use a windowed low point averaging algorithm as can be
 * found in Fober, Orlarey and Letz, 2005, "Real Time Clock Skew Estimation
 * over Network Delays":
 * http://www.grame.fr/Ressources/pub/TR-050601.pdf
 * http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.102.1546
 *
 * The idea is that the jitter is composed of:
 *
 *  J = N + n
 *
 *   N   : a constant network delay.
 *   n   : random added noise. The noise is concentrated around 0
 *
 * In the receiver we can track the elapsed time at the sender with:
 *
 *  send_diff(i) = (Tsi - Ts0);
 *
 *   Tsi : The time at the sender at packet i
 *   Ts0 : The time at the sender at the first packet
 *
 * This is the difference between the RTP timestamp in the first received packet
 * and the current packet.
 *
 * At the receiver we have to deal with the jitter introduced by the network.
 *
 *  recv_diff(i) = (Tri - Tr0)
 *
 *   Tri : The time at the receiver at packet i
 *   Tr0 : The time at the receiver at the first packet
 *
 * Both of these values contain a jitter Ji, a jitter for packet i, so we can
 * write:
 *
 *  recv_diff(i) = (Cri + D + ni) - (Cr0 + D + n0))
 *
 *    Cri    : The time of the clock at the receiver for packet i
 *    D + ni : The jitter when receiving packet i
 *
 * We see that the network delay is irrelevant here as we can elliminate D:
 *
 *  recv_diff(i) = (Cri + ni) - (Cr0 + n0))
 *
 * The drift is now expressed as:
 *
 *  Drift(i) = recv_diff(i) - send_diff(i);
 *
 * We now keep the W latest values of Drift and find the minimum (this is the
 * one with the lowest network jitter and thus the one which is least affected
 * by it). We average this lowest value to smooth out the resulting network skew.
 *
 * Both the window and the weighting used for averaging influence the accuracy
 * of the drift estimation. Finding the correct parameters turns out to be a
 * compromise between accuracy and inertia.
 *
 * We use a 2 second window or up to 512 data points, which is statistically big
 * enough to catch spikes (FIXME, detect spikes).
 * We also use a rather large weighting factor (125) to smoothly adapt. During
 * startup, when filling the window, we use a parabolic weighting factor, the
 * more the window is filled, the faster we move to the detected possible skew.
 *
 * Returns: @time adjusted with the clock skew.
 */
static GstClockTime
calculate_skew (RTPJitterBuffer * jbuf, guint32 rtptime, GstClockTime time)
{
  guint64 ext_rtptime;
  guint64 send_diff, recv_diff;
  gint64 delta;
  gint64 old;
  gint pos, i;
  GstClockTime gstrtptime, out_time;
  guint64 slope;

  ext_rtptime = gst_rtp_buffer_ext_timestamp (&jbuf->ext_rtptime, rtptime);

  if (jbuf->last_rtptime != -1 && ext_rtptime == jbuf->last_rtptime)
    return jbuf->prev_out_time;

  gstrtptime =
      gst_util_uint64_scale_int (ext_rtptime, GST_SECOND, jbuf->clock_rate);

  /* keep track of the last extended rtptime */
  jbuf->last_rtptime = ext_rtptime;

  send_diff = 0;
  if (G_LIKELY (jbuf->base_rtptime != -1)) {
    /* check elapsed time in RTP units */
    if (G_LIKELY (gstrtptime >= jbuf->base_rtptime)) {
      send_diff = gstrtptime - jbuf->base_rtptime;
    } else {
      /* elapsed time at sender, timestamps can go backwards and thus be
       * smaller than our base time, schedule to take a new base time in
       * that case. */
      GST_WARNING ("backward timestamps at server, schedule resync");
      jbuf->need_resync = TRUE;
      send_diff = 0;
    }
  }

  /* need resync, lock on to time and gstrtptime if we can, otherwise we
   * do with the previous values */
  if (G_UNLIKELY (jbuf->need_resync && time != -1)) {
    GST_INFO ("resync to time %" GST_TIME_FORMAT ", rtptime %"
        GST_TIME_FORMAT, GST_TIME_ARGS (time), GST_TIME_ARGS (gstrtptime));
    rtp_jitter_buffer_resync (jbuf, time, gstrtptime, ext_rtptime, FALSE);
    send_diff = 0;
  }

  GST_DEBUG ("extrtp %" G_GUINT64_FORMAT ", gstrtp %" GST_TIME_FORMAT ", base %"
      GST_TIME_FORMAT ", send_diff %" GST_TIME_FORMAT, ext_rtptime,
      GST_TIME_ARGS (gstrtptime), GST_TIME_ARGS (jbuf->base_rtptime),
      GST_TIME_ARGS (send_diff));

  /* we don't have an arrival timestamp so we can't do skew detection. we
   * should still apply a timestamp based on RTP timestamp and base_time */
  if (time == -1 || jbuf->base_time == -1)
    goto no_skew;

  /* elapsed time at receiver, includes the jitter */
  recv_diff = time - jbuf->base_time;

  /* measure the diff */
  delta = ((gint64) recv_diff) - ((gint64) send_diff);

  /* measure the slope, this gives a rought estimate between the sender speed
   * and the receiver speed. This should be approximately 8, higher values
   * indicate a burst (especially when the connection starts) */
  if (recv_diff > 0)
    slope = (send_diff * 8) / recv_diff;
  else
    slope = 8;

  GST_DEBUG ("time %" GST_TIME_FORMAT ", base %" GST_TIME_FORMAT ", recv_diff %"
      GST_TIME_FORMAT ", slope %" G_GUINT64_FORMAT, GST_TIME_ARGS (time),
      GST_TIME_ARGS (jbuf->base_time), GST_TIME_ARGS (recv_diff), slope);

  /* if the difference between the sender timeline and the receiver timeline
   * changed too quickly we have to resync because the server likely restarted
   * its timestamps. */
  if (ABS (delta - jbuf->skew) > GST_SECOND) {
    GST_WARNING ("delta - skew: %" GST_TIME_FORMAT " too big, reset skew",
        GST_TIME_ARGS (ABS (delta - jbuf->skew)));
    rtp_jitter_buffer_resync (jbuf, time, gstrtptime, ext_rtptime, TRUE);
    send_diff = 0;
    delta = 0;
  }

  pos = jbuf->window_pos;

  if (G_UNLIKELY (jbuf->window_filling)) {
    /* we are filling the window */
    GST_DEBUG ("filling %d, delta %" G_GINT64_FORMAT, pos, delta);
    jbuf->window[pos++] = delta;
    /* calc the min delta we observed */
    if (G_UNLIKELY (pos == 1 || delta < jbuf->window_min))
      jbuf->window_min = delta;

    if (G_UNLIKELY (send_diff >= MAX_TIME || pos >= MAX_WINDOW)) {
      jbuf->window_size = pos;

      /* window filled */
      GST_DEBUG ("min %" G_GINT64_FORMAT, jbuf->window_min);

      /* the skew is now the min */
      jbuf->skew = jbuf->window_min;
      jbuf->window_filling = FALSE;
    } else {
      gint perc_time, perc_window, perc;

      /* figure out how much we filled the window, this depends on the amount of
       * time we have or the max number of points we keep. */
      perc_time = send_diff * 100 / MAX_TIME;
      perc_window = pos * 100 / MAX_WINDOW;
      perc = MAX (perc_time, perc_window);

      /* make a parabolic function, the closer we get to the MAX, the more value
       * we give to the scaling factor of the new value */
      perc = perc * perc;

      /* quickly go to the min value when we are filling up, slowly when we are
       * just starting because we're not sure it's a good value yet. */
      jbuf->skew =
          (perc * jbuf->window_min + ((10000 - perc) * jbuf->skew)) / 10000;
      jbuf->window_size = pos + 1;
    }
  } else {
    /* pick old value and store new value. We keep the previous value in order
     * to quickly check if the min of the window changed */
    old = jbuf->window[pos];
    jbuf->window[pos++] = delta;

    if (G_UNLIKELY (delta <= jbuf->window_min)) {
      /* if the new value we inserted is smaller or equal to the current min,
       * it becomes the new min */
      jbuf->window_min = delta;
    } else if (G_UNLIKELY (old == jbuf->window_min)) {
      gint64 min = G_MAXINT64;

      /* if we removed the old min, we have to find a new min */
      for (i = 0; i < jbuf->window_size; i++) {
        /* we found another value equal to the old min, we can stop searching now */
        if (jbuf->window[i] == old) {
          min = old;
          break;
        }
        if (jbuf->window[i] < min)
          min = jbuf->window[i];
      }
      jbuf->window_min = min;
    }
    /* average the min values */
    jbuf->skew = (jbuf->window_min + (124 * jbuf->skew)) / 125;
    GST_DEBUG ("delta %" G_GINT64_FORMAT ", new min: %" G_GINT64_FORMAT,
        delta, jbuf->window_min);
  }
  /* wrap around in the window */
  if (G_UNLIKELY (pos >= jbuf->window_size))
    pos = 0;
  jbuf->window_pos = pos;

no_skew:
  /* the output time is defined as the base timestamp plus the RTP time
   * adjusted for the clock skew .*/
  if (jbuf->base_time != -1) {
    out_time = jbuf->base_time + send_diff;
    /* skew can be negative and we don't want to make invalid timestamps */
    if (jbuf->skew < 0 && out_time < -jbuf->skew) {
      out_time = 0;
    } else {
      out_time += jbuf->skew;
    }
    /* check if timestamps are not going backwards, we can only check this if we
     * have a previous out time and a previous send_diff */
    if (G_LIKELY (jbuf->prev_out_time != -1 && jbuf->prev_send_diff != -1)) {
      /* now check for backwards timestamps */
      if (G_UNLIKELY (
              /* if the server timestamps went up and the out_time backwards */
              (send_diff > jbuf->prev_send_diff
                  && out_time < jbuf->prev_out_time) ||
              /* if the server timestamps went backwards and the out_time forwards */
              (send_diff < jbuf->prev_send_diff
                  && out_time > jbuf->prev_out_time) ||
              /* if the server timestamps did not change */
              send_diff == jbuf->prev_send_diff)) {
        GST_DEBUG ("backwards timestamps, using previous time");
        out_time = jbuf->prev_out_time;
      }
    }
    if (time != -1 && out_time + jbuf->delay < time) {
      /* if we are going to produce a timestamp that is later than the input
       * timestamp, we need to reset the jitterbuffer. Likely the server paused
       * temporarily */
      GST_DEBUG ("out %" GST_TIME_FORMAT " + %" G_GUINT64_FORMAT " < time %"
          GST_TIME_FORMAT ", reset jitterbuffer", GST_TIME_ARGS (out_time),
          jbuf->delay, GST_TIME_ARGS (time));
      rtp_jitter_buffer_resync (jbuf, time, gstrtptime, ext_rtptime, TRUE);
      out_time = time;
      send_diff = 0;
    }
  } else
    out_time = -1;

  jbuf->prev_out_time = out_time;
  jbuf->prev_send_diff = send_diff;

  GST_DEBUG ("skew %" G_GINT64_FORMAT ", out %" GST_TIME_FORMAT,
      jbuf->skew, GST_TIME_ARGS (out_time));

  return out_time;
}

static void
queue_do_insert (RTPJitterBuffer * jbuf, GList * list, GList * item)
{
  GQueue *queue = jbuf->packets;

  /* It's more likely that the packet was inserted at the tail of the queue */
  if (G_LIKELY (list)) {
    item->prev = list;
    item->next = list->next;
    list->next = item;
  } else {
    item->prev = NULL;
    item->next = queue->head;
    queue->head = item;
  }
  if (item->next)
    item->next->prev = item;
  else
    queue->tail = item;
  queue->length++;
}

/**
 * rtp_jitter_buffer_insert:
 * @jbuf: an #RTPJitterBuffer
 * @item: an #RTPJitterBufferItem to insert
 * @head: TRUE when the head element changed.
 * @percent: the buffering percent after insertion
 *
 * Inserts @item into the packet queue of @jbuf. The sequence number of the
 * packet will be used to sort the packets. This function takes ownerhip of
 * @buf when the function returns %TRUE.
 *
 * When @head is %TRUE, the new packet was added at the head of the queue and
 * will be available with the next call to rtp_jitter_buffer_pop() and
 * rtp_jitter_buffer_peek().
 *
 * Returns: %FALSE if a packet with the same number already existed.
 */
gboolean
rtp_jitter_buffer_insert (RTPJitterBuffer * jbuf, RTPJitterBufferItem * item,
    gboolean * head, gint * percent)
{
  GList *list, *event = NULL;
  guint32 rtptime;
  guint16 seqnum;
  GstClockTime dts;

  g_return_val_if_fail (jbuf != NULL, FALSE);
  g_return_val_if_fail (item != NULL, FALSE);

  list = jbuf->packets->tail;

  /* no seqnum, simply append then */
  if (item->seqnum == -1)
    goto append;

  seqnum = item->seqnum;

  /* loop the list to skip strictly larger seqnum buffers */
  for (; list; list = g_list_previous (list)) {
    guint16 qseq;
    gint gap;
    RTPJitterBufferItem *qitem = (RTPJitterBufferItem *) list;

    if (qitem->seqnum == -1) {
      /* keep a pointer to the first consecutive event if not already
       * set. we will insert the packet after the event if we can't find
       * a packet with lower sequence number before the event. */
      if (event == NULL)
        event = list;
      continue;
    }

    qseq = qitem->seqnum;

    /* compare the new seqnum to the one in the buffer */
    gap = gst_rtp_buffer_compare_seqnum (seqnum, qseq);

    /* we hit a packet with the same seqnum, notify a duplicate */
    if (G_UNLIKELY (gap == 0))
      goto duplicate;

    /* seqnum > qseq, we can stop looking */
    if (G_LIKELY (gap < 0))
      break;

    /* if we've found a packet with greater sequence number, cleanup the
     * event pointer as the packet will be inserted before the event */
    event = NULL;
  }

  /* if event is set it means that packets before the event had smaller
   * sequence number, so we will insert our packet after the event */
  if (event)
    list = event;

  dts = item->dts;
  if (item->rtptime == -1)
    goto append;

  rtptime = item->rtptime;

  /* rtp time jumps are checked for during skew calculation, but bypassed
   * in other mode, so mind those here and reset jb if needed.
   * Only reset if valid input time, which is likely for UDP input
   * where we expect this might happen due to async thread effects
   * (in seek and state change cycles), but not so much for TCP input */
  if (GST_CLOCK_TIME_IS_VALID (dts) &&
      jbuf->mode != RTP_JITTER_BUFFER_MODE_SLAVE &&
      jbuf->base_time != -1 && jbuf->last_rtptime != -1) {
    GstClockTime ext_rtptime = jbuf->ext_rtptime;

    ext_rtptime = gst_rtp_buffer_ext_timestamp (&ext_rtptime, rtptime);
    if (ext_rtptime > jbuf->last_rtptime + 3 * jbuf->clock_rate ||
        ext_rtptime + 3 * jbuf->clock_rate < jbuf->last_rtptime) {
      /* reset even if we don't have valid incoming time;
       * still better than producing possibly very bogus output timestamp */
      GST_WARNING ("rtp delta too big, reset skew");
      rtp_jitter_buffer_reset_skew (jbuf);
    }
  }

  switch (jbuf->mode) {
    case RTP_JITTER_BUFFER_MODE_NONE:
    case RTP_JITTER_BUFFER_MODE_BUFFER:
      /* send 0 as the first timestamp and -1 for the other ones. This will
       * interpolate them from the RTP timestamps with a 0 origin. In buffering
       * mode we will adjust the outgoing timestamps according to the amount of
       * time we spent buffering. */
      if (jbuf->base_time == -1)
        dts = 0;
      else
        dts = -1;
      break;
    case RTP_JITTER_BUFFER_MODE_SYNCED:
      /* synchronized clocks, take first timestamp as base, use RTP timestamps
       * to interpolate */
      if (jbuf->base_time != -1)
        dts = -1;
      break;
    case RTP_JITTER_BUFFER_MODE_SLAVE:
    default:
      break;
  }
  /* do skew calculation by measuring the difference between rtptime and the
   * receive dts, this function will return the skew corrected rtptime. */
  item->pts = calculate_skew (jbuf, rtptime, dts);

append:
  queue_do_insert (jbuf, list, (GList *) item);

  /* buffering mode, update buffer stats */
  if (jbuf->mode == RTP_JITTER_BUFFER_MODE_BUFFER)
    update_buffer_level (jbuf, percent);
  else if (percent)
    *percent = -1;

  /* head was changed when we did not find a previous packet, we set the return
   * flag when requested. */
  if (G_LIKELY (head))
    *head = (list == NULL);

  return TRUE;

  /* ERRORS */
duplicate:
  {
    GST_WARNING ("duplicate packet %d found", (gint) seqnum);
    return FALSE;
  }
}

/**
 * rtp_jitter_buffer_pop:
 * @jbuf: an #RTPJitterBuffer
 * @percent: the buffering percent
 *
 * Pops the oldest buffer from the packet queue of @jbuf. The popped buffer will
 * have its timestamp adjusted with the incomming running_time and the detected
 * clock skew.
 *
 * Returns: a #GstBuffer or %NULL when there was no packet in the queue.
 */
RTPJitterBufferItem *
rtp_jitter_buffer_pop (RTPJitterBuffer * jbuf, gint * percent)
{
  GList *item = NULL;
  GQueue *queue;

  g_return_val_if_fail (jbuf != NULL, NULL);

  queue = jbuf->packets;

  item = queue->head;
  if (item) {
    queue->head = item->next;
    if (queue->head)
      queue->head->prev = NULL;
    else
      queue->tail = NULL;
    queue->length--;
  }

  /* buffering mode, update buffer stats */
  if (jbuf->mode == RTP_JITTER_BUFFER_MODE_BUFFER)
    update_buffer_level (jbuf, percent);
  else if (percent)
    *percent = -1;

  return (RTPJitterBufferItem *) item;
}

/**
 * rtp_jitter_buffer_peek:
 * @jbuf: an #RTPJitterBuffer
 *
 * Peek the oldest buffer from the packet queue of @jbuf.
 *
 * See rtp_jitter_buffer_insert() to check when an older packet was
 * added.
 *
 * Returns: a #GstBuffer or %NULL when there was no packet in the queue.
 */
RTPJitterBufferItem *
rtp_jitter_buffer_peek (RTPJitterBuffer * jbuf)
{
  g_return_val_if_fail (jbuf != NULL, NULL);

  return (RTPJitterBufferItem *) jbuf->packets->head;
}

/**
 * rtp_jitter_buffer_flush:
 * @jbuf: an #RTPJitterBuffer
 * @free_func: function to free each item
 * @user_data: user data passed to @free_func
 *
 * Flush all packets from the jitterbuffer.
 */
void
rtp_jitter_buffer_flush (RTPJitterBuffer * jbuf, GFunc free_func,
    gpointer user_data)
{
  GList *item;

  g_return_if_fail (jbuf != NULL);
  g_return_if_fail (free_func != NULL);

  while ((item = g_queue_pop_head_link (jbuf->packets)))
    free_func ((RTPJitterBufferItem *) item, user_data);
}

/**
 * rtp_jitter_buffer_is_buffering:
 * @jbuf: an #RTPJitterBuffer
 *
 * Check if @jbuf is buffering currently. Users of the jitterbuffer should not
 * pop packets while in buffering mode.
 *
 * Returns: the buffering state of @jbuf
 */
gboolean
rtp_jitter_buffer_is_buffering (RTPJitterBuffer * jbuf)
{
  return jbuf->buffering && !jbuf->buffering_disabled;
}

/**
 * rtp_jitter_buffer_set_buffering:
 * @jbuf: an #RTPJitterBuffer
 * @buffering: the new buffering state
 *
 * Forces @jbuf to go into the buffering state.
 */
void
rtp_jitter_buffer_set_buffering (RTPJitterBuffer * jbuf, gboolean buffering)
{
  jbuf->buffering = buffering;
}

/**
 * rtp_jitter_buffer_get_percent:
 * @jbuf: an #RTPJitterBuffer
 *
 * Get the buffering percent of the jitterbuffer.
 *
 * Returns: the buffering percent
 */
gint
rtp_jitter_buffer_get_percent (RTPJitterBuffer * jbuf)
{
  gint percent;
  guint64 level;

  if (G_UNLIKELY (jbuf->high_level == 0))
    return 100;

  if (G_UNLIKELY (jbuf->buffering_disabled))
    return 100;

  level = get_buffer_level (jbuf);
  percent = (level * 100 / jbuf->high_level);
  percent = MIN (percent, 100);

  return percent;
}

/**
 * rtp_jitter_buffer_num_packets:
 * @jbuf: an #RTPJitterBuffer
 *
 * Get the number of packets currently in "jbuf.
 *
 * Returns: The number of packets in @jbuf.
 */
guint
rtp_jitter_buffer_num_packets (RTPJitterBuffer * jbuf)
{
  g_return_val_if_fail (jbuf != NULL, 0);

  return jbuf->packets->length;
}

/**
 * rtp_jitter_buffer_get_ts_diff:
 * @jbuf: an #RTPJitterBuffer
 *
 * Get the difference between the timestamps of first and last packet in the
 * jitterbuffer.
 *
 * Returns: The difference expressed in the timestamp units of the packets.
 */
guint32
rtp_jitter_buffer_get_ts_diff (RTPJitterBuffer * jbuf)
{
  guint64 high_ts, low_ts;
  RTPJitterBufferItem *high_buf, *low_buf;
  guint32 result;

  g_return_val_if_fail (jbuf != NULL, 0);

  high_buf = (RTPJitterBufferItem *) g_queue_peek_head_link (jbuf->packets);
  low_buf = (RTPJitterBufferItem *) g_queue_peek_tail_link (jbuf->packets);

  if (!high_buf || !low_buf || high_buf == low_buf)
    return 0;

  high_ts = high_buf->rtptime;
  low_ts = low_buf->rtptime;

  /* it needs to work if ts wraps */
  if (high_ts >= low_ts) {
    result = (guint32) (high_ts - low_ts);
  } else {
    result = (guint32) (high_ts + G_MAXUINT32 + 1 - low_ts);
  }
  return result;
}

/**
 * rtp_jitter_buffer_get_sync:
 * @jbuf: an #RTPJitterBuffer
 * @rtptime: result RTP time
 * @timestamp: result GStreamer timestamp
 * @clock_rate: clock-rate of @rtptime
 * @last_rtptime: last seen rtptime.
 *
 * Calculates the relation between the RTP timestamp and the GStreamer timestamp
 * used for constructing timestamps.
 *
 * For extended RTP timestamp @rtptime with a clock-rate of @clock_rate,
 * the GStreamer timestamp is currently @timestamp.
 *
 * The last seen extended RTP timestamp with clock-rate @clock-rate is returned in
 * @last_rtptime.
 */
void
rtp_jitter_buffer_get_sync (RTPJitterBuffer * jbuf, guint64 * rtptime,
    guint64 * timestamp, guint32 * clock_rate, guint64 * last_rtptime)
{
  if (rtptime)
    *rtptime = jbuf->base_extrtp;
  if (timestamp)
    *timestamp = jbuf->base_time + jbuf->skew;
  if (clock_rate)
    *clock_rate = jbuf->clock_rate;
  if (last_rtptime)
    *last_rtptime = jbuf->last_rtptime;
}
