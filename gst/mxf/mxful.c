/* GStreamer
 * Copyright (C) <2009> Sebastian Dröge <sebastian.droege@collabora.co.uk>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include "mxful.h"

const MXFUL _mxf_ul_table[] = {
  /* SMPTE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x00,}},
  /* FILL, SMPTE 336M */
  {{0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x03, 0x01, 0x02, 0x10, 0x01, 0x00,}},
  /* PARTITION_PACK, SMPTE 377M 6.1 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
          0x0d, 0x01, 0x02, 0x01, 0x01, 0x00,}},
  /* PRIMER_PACK, SMPTE 377M 8.1 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
          0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00}},
  /* METADATA, SMPTE 377M 8.6 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
          0x0d, 0x01, 0x01, 0x01, 0x01, 0x00,}},
  /* DESCRIPTIVE_METADATA, SMPTE 377M 8.7.3 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x00, 0x01, 0x01,
          0x0d, 0x01, 0x04, 0x01, 0x00,}},
  /* RANDOM_INDEX_PACK, SMPTE 377M 11.1 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
          0x0d, 0x01, 0x02, 0x01, 0x01, 0x11, 0x01, 0x00}},
  /* INDEX_TABLE_SEGMENT, SMPTE 377M 10.2.2 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
          0x0d, 0x01, 0x02, 0x01, 0x01, 0x10, 0x01, 0x00}},
  /* GENERIC_CONTAINER_SYSTEM_ITEM, SMPTE 379M 6.2.1 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x02, 0x00, 0x01, 0x00,
          0x0d, 0x01, 0x03, 0x01, 0x00}},
  /* GENERIC_CONTAINER_ESSENCE_ELEMENT, SMPTE 379M 7.1 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x00,
          0x0d, 0x01, 0x03, 0x01, 0x00,}},
  /* GENERIC_CONTAINER_ESSENCE_CONTAINER_LABEL, SMPTE 379M 8 */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x00,
          0x0d, 0x01, 0x03, 0x01, 0x00,}},
  /* AVID_ESSENCE_CONTAINER_ESSENCE_ELEMENT, undocumented */
  {{0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01,
          0x0e, 0x04, 0x03, 0x01, 0x00,}},
  /* AVID_ESSENCE_CONTAINER_ESSENCE_LABEL, undocumented */
  {{0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff,
          0x4b, 0x46, 0x41, 0x41, 0x00, 0x0d, 0x4d, 0x4f}},
  /* OPERATIONAL_PATTERN_IDENTIFICATION */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x0d, 0x01, 0x02, 0x01, 0x00,}},
  /* TRACK_TIMECODE_12M_INACTIVE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00}},
  /* TRACK_TIMECODE_12M_ACTIVE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00}},
  /* TRACK_TIMECODE_309M */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00}},
  /* TRACK_METADATA */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x01, 0x10, 0x00, 0x00, 0x00}},
  /* TRACK_PICTURE_ESSENCE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00}},
  /* TRACK_SOUND_ESSENCE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00}},
  /* TRACK_DATA_ESSENCE */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00}},
  /* TRACK_AUXILIARY_DATA */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00}},
  /* TRACK_PARSED_TEXT */
  {{0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x02, 0x03, 0x02, 0x00, 0x00, 0x00}},
  /* TRACK_AVID_PICTURE_ESSENCE */
  {{0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f,
          0x6f, 0x3c, 0x8c, 0xe1, 0x6c, 0xef, 0x11, 0xd2}},
  /** MXF Structural Metadata */
  /* INSTANCE_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x01, 0x01, 0x15, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* GENERATION_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00}},
  /* Preface */
  /* LAST_MODIFIED_DATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x10, 0x02, 0x04, 0x00, 0x00}},
  /* VERSION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x03, 0x01, 0x02, 0x01, 0x05, 0x00, 0x00, 0x00}},
  /* OBJECT_MODEL_VERSION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x03, 0x01, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00}},
  /* PRIMARY_PACKAGE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x06, 0x01, 0x01, 0x04, 0x01, 0x08, 0x00, 0x00}},
  /* IDENTIFICATIONS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x04, 0x00, 0x00}},
  /* CONTENT_STORAGE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x02, 0x01, 0x00, 0x00}},
  /* OPERATIONAL_PATTERN */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x01, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00}},
  /* ESSENCE_CONTAINERS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x01, 0x02, 0x02, 0x10, 0x02, 0x01, 0x00, 0x00}},
  /* DM_SCHEMES */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x01, 0x02, 0x02, 0x10, 0x02, 0x02, 0x00, 0x00}},
  /* Identification */
  /* THIS_GENERATION_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00}},
  /* COMPANY_NAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x02, 0x01, 0x00, 0x00}},
  /* PRODUCT_NAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x03, 0x01, 0x00, 0x00}},
  /* PRODUCT_VERSION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x04, 0x00, 0x00, 0x00}},
  /* VERSION_STRING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x05, 0x01, 0x00, 0x00}},
  /* PRODUCT_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x07, 0x00, 0x00, 0x00}},
  /* MODIFICATION_DATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x10, 0x02, 0x03, 0x00, 0x00}},
  /* TOOLKIT_VERSION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x0A, 0x00, 0x00, 0x00}},
  /* PLATFORM */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x07, 0x01, 0x06, 0x01, 0x00, 0x00}},
  /* Content storage */
  /* PACKAGES */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x05, 0x01, 0x00, 0x00}},
  /* ESSENCE_CONTAINER_DATA */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x05, 0x02, 0x00, 0x00}},
  /* Essence container data */
  /* LINKED_PACKAGE_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x06, 0x01, 0x00, 0x00, 0x00}},
  /* BODY_SID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x01, 0x03, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00}},
  /* INDEX_SID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x01, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00}},
  /* Generic package */
  /* PACKAGE_UID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x01, 0x01, 0x15, 0x10, 0x00, 0x00, 0x00, 0x00}},
  /* PACKAGE_NAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x01, 0x03, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00}},
  /* PACKAGE_CREATION_DATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x10, 0x01, 0x03, 0x00, 0x00}},
  /* PACKAGE_MODIFIED_DATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x10, 0x02, 0x05, 0x00, 0x00}},
  /* TRACKS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x05, 0x00, 0x00}},
  /* Source package */
  /* DESCRIPTOR */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x02, 0x03, 0x00, 0x00}},
  /* Track */
  /* TRACK_ID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x01, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* TRACK_NUMBER */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x01, 0x04, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00}},
  /* TRACK_NAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x01, 0x07, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00}},
  /* SEQUENCE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x02, 0x04, 0x00, 0x00}},
  /* Timeline track */
  /* EDIT_RATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x30, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00}},
  /* ORIGIN */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x03, 0x01, 0x03, 0x00, 0x00}},
  /* Event track */
  /* EVENT_EDIT_RATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x30, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* EVENT_ORIGIN */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x07, 0x02, 0x01, 0x03, 0x01, 0x0B, 0x00, 0x00}},
  /* Sequence & structural component */
  /* DATA_DEFINITION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}},
  /* DURATION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x02, 0x01, 0x01, 0x03, 0x00, 0x00}},
  /* STRUCTURAL_COMPONENTS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x09, 0x00, 0x00}},
  /* Timecode component */
  /* ROUNDED_TIMECODE_BASE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x04, 0x01, 0x01, 0x02, 0x06, 0x00, 0x00}},
  /* START_TIMECODE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x03, 0x01, 0x05, 0x00, 0x00}},
  /* DROP_FRAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x04, 0x01, 0x01, 0x05, 0x00, 0x00, 0x00}},
  /* Source clip */
  /* START_POSITION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x03, 0x01, 0x04, 0x00, 0x00}},
  /* SOURCE_PACKAGE_ID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00}},
  /* SOURCE_TRACK_ID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x03, 0x02, 0x00, 0x00, 0x0}},
  /* DM Source clip */
  /* DM_SOURCELIP_TRACK_IDS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x01, 0x07, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00}},
  /* DM Segment */
  /* EVENT_START_POSITION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x07, 0x02, 0x01, 0x03, 0x03, 0x03, 0x00, 0x00}},
  /* EVENT_COMMENT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x30, 0x04, 0x04, 0x01, 0x00, 0x00, 0x00}},
  /* DM_SEGMENT_TRACK_IDS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x01, 0x07, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00}},
  /* DM_FRAMEWORK */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x06, 0x01, 0x01, 0x04, 0x02, 0x0C, 0x00, 0x00}},
  /* Generic descriptor */
  /* LOCATORS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x03, 0x00, 0x00}},
  /* SUB_DESCRIPTORS_ARRAY */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x09,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x10, 0x00, 0x00}},
  /* File descriptor */
  /* LINKED_TRACK_ID */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x06, 0x01, 0x01, 0x03, 0x05, 0x00, 0x00, 0x00}},
  /* SAMPLE_RATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* CONTAINER_DURATION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x06, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* ESSENCE_CONTAINER */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x01, 0x02, 0x00, 0x00}},
  /* CODEC */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x06, 0x01, 0x01, 0x04, 0x01, 0x03, 0x00, 0x00}},
  /* Generic picture essence descriptor */
  /* SIGNAL_STANDARD */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x05, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00}},
  /* FRAME_LAYOUT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x03, 0x01, 0x04, 0x00, 0x00, 0x00}},
  /* STORED_WIDTH */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x02, 0x02, 0x00, 0x00, 0x00}},
  /* STORED_HEIGHT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x02, 0x01, 0x00, 0x00, 0x00}},
  /* STORED_F2_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x03, 0x02, 0x08, 0x00, 0x00, 0x00}},
  /* SAMPLED_WIDTH */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x08, 0x00, 0x00, 0x00}},
  /* SAMPLED_HEIGHT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x07, 0x00, 0x00, 0x00}},
  /* SAMPLED_X_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x09, 0x00, 0x00, 0x00}},
  /* SAMPLED_Y_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x0A, 0x00, 0x00, 0x00}},
  /* DISPLAY_HEIGHT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x0B, 0x00, 0x00, 0x00}},
  /* DISPLAY_WIDTH */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x0C, 0x00, 0x00, 0x00}},
  /* DISPLAY_X_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x0D, 0x00, 0x00, 0x00}},
  /* DISPLAY_Y_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x0E, 0x00, 0x00, 0x00}},
  /* DISPLAY_F2_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x03, 0x02, 0x07, 0x00, 0x00, 0x00}},
  /* ASPECT_RATIO */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}},
  /* ACTIVE_FORMAT_DESCRIPTOR */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x03, 0x02, 0x09, 0x00, 0x00, 0x00}},
  /* VIDEO_LINE_MAP */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x03, 0x02, 0x05, 0x00, 0x00, 0x00}},
  /* ALPHA_TRANSPARENCY */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x05, 0x20, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* CAPTURE_GAMMA */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 0x00}},
  /* IMAGE_ALIGNMENT_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x18, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* IMAGE_START_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x18, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* IMAGE_END_OFFSET */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x18, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* FIELD_DOMINANCE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x03, 0x01, 0x06, 0x00, 0x00, 0x00}},
  /* PICTURE_ESSENCE_CODING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* Generic audio essence descriptor */
  /* AUDIO_SAMPLING_RATE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x02, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00}},
  /* LOCKED */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x04, 0x02, 0x03, 0x01, 0x04, 0x00, 0x00, 0x00}},
  /* AUDIO_REF_LEVEL */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x02, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00}},
  /* ELECTRO_SPATIAL_FORMULATION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}},
  /* CHANNEL_COUNT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x02, 0x01, 0x01, 0x04, 0x00, 0x00, 0x00}},
  /* QUANTIZATION_BITS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x04, 0x02, 0x03, 0x03, 0x04, 0x00, 0x00, 0x00}},
  /* DIAL_NORM */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x02, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00}},
  /* SOUND_ESSENCE_COMPRESSION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x02, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* CDCI Picture essence descriptor */
  /* COMPONENT_DEPTH */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x05, 0x03, 0x0A, 0x00, 0x00, 0x00}},
  /* HORIZONTAL_SUBSAMPLING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x05, 0x00, 0x00, 0x00}},
  /* VERTICAL_SUBSAMPLING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x05, 0x01, 0x10, 0x00, 0x00, 0x00}},
  /* COLOR_SITING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x01, 0x06, 0x00, 0x00, 0x00}},
  /* REVERSED_BYTE_ORDER */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x03, 0x01, 0x02, 0x01, 0x0A, 0x00, 0x00, 0x00}},
  /* PADDING_BITS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x18, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00}},
  /* ALPHA_SAMPLE_DEPTH */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x05, 0x03, 0x07, 0x00, 0x00, 0x00}},
  /* BLACK_REF_LEVEL */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x03, 0x03, 0x00, 0x00, 0x00}},
  /* WHITE_REF_LEVEL */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x04, 0x01, 0x05, 0x03, 0x04, 0x00, 0x00, 0x00}},
  /* COLOR_RANGE */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x05, 0x03, 0x05, 0x00, 0x00, 0x00}},
  /* RGBA picture essence descriptor */
  /* COMPONENT_MAX_REF */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x05, 0x03, 0x0B, 0x00, 0x00, 0x00}},
  /* COMPONENT_MIN_REF */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x05, 0x03, 0x0C, 0x00, 0x00, 0x00}},
  /* ALPHA_MAX_REF */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x05, 0x03, 0x0D, 0x00, 0x00, 0x00}},
  /* ALPHA_MIN_REF */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x05, 0x03, 0x0E, 0x00, 0x00, 0x00}},
  /* SCANNING_DIRECTION */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05,
          0x04, 0x01, 0x04, 0x04, 0x01, 0x00, 0x00, 0x00}},
  /* PIXEL_LAYOUT */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x04, 0x01, 0x05, 0x03, 0x06, 0x00, 0x00, 0x00}},
  /* Generic data essence descriptor */
  /* DATA_ESSENCE_CODING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03,
          0x04, 0x03, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00}},
  /* Multiple descriptor */
  /* SUB_DESCRIPTORS */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04,
          0x06, 0x01, 0x01, 0x04, 0x06, 0x0B, 0x00, 0x00}},
  /* Text locator */
  /* LOCATOR_NAME */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02,
          0x01, 0x04, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00}},
  /* Network locator */
  /* URL_STRING */
  {{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
          0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}},
};

gboolean
mxf_ul_is_equal (const MXFUL * a, const MXFUL * b)
{
  guint i;

  g_return_val_if_fail (a != NULL, FALSE);
  g_return_val_if_fail (b != NULL, FALSE);

  for (i = 0; i < 16; i++) {
    /* registry version */
    if (i == 7)
      continue;

    if (a->u[i] != b->u[i])
      return FALSE;
  }

  return TRUE;
}

gboolean
mxf_ul_is_subclass (const MXFUL * class, const MXFUL * subclass)
{
  guint i;

  g_return_val_if_fail (class != NULL, FALSE);
  g_return_val_if_fail (subclass != NULL, FALSE);

  for (i = 0; i < 16; i++) {
    if (i == 7)
      /* registry version */
      continue;

    if (class->u[i] != 0x00 && class->u[i] != subclass->u[i])
      return FALSE;
  }

  return TRUE;
}

gboolean
mxf_ul_is_zero (const MXFUL * ul)
{
  static const guint8 zero[16] = { 0, };

  g_return_val_if_fail (ul != NULL, FALSE);

  return (memcmp (ul, &zero, 16) == 0);
}

gboolean
mxf_ul_is_valid (const MXFUL * ul)
{
  guint i, j;

  g_return_val_if_fail (ul != NULL, FALSE);

  for (i = 0; i < 16; i++) {
    if (ul->u[i] == 0x00) {
      for (j = i; j < 16; j++) {
        if (ul->u[j] != 0x00)
          return FALSE;
      }

      return TRUE;
    }

    if (ul->u[i] > 0x7f)
      return FALSE;
  }

  return TRUE;
}

guint
mxf_ul_hash (const MXFUL * ul)
{
  guint32 ret = 0;
  guint i;

  g_return_val_if_fail (ul != NULL, 0);

  for (i = 0; i < 4; i++)
    ret ^= (ul->u[i * 4 + 0] << 24) |
        (ul->u[i * 4 + 1] << 16) |
        (ul->u[i * 4 + 2] << 8) | (ul->u[i * 4 + 3] << 0);

  return ret;
}

gchar *
mxf_ul_to_string (const MXFUL * ul, gchar str[48])
{
  gchar *ret = str;

  g_return_val_if_fail (ul != NULL, NULL);

  if (ret == NULL)
    ret = g_malloc (48);

  g_snprintf (ret, 48,
      "%02x.%02x.%02x.%02x."
      "%02x.%02x.%02x.%02x."
      "%02x.%02x.%02x.%02x."
      "%02x.%02x.%02x.%02x",
      ul->u[0], ul->u[1], ul->u[2], ul->u[3],
      ul->u[4], ul->u[5], ul->u[6], ul->u[7],
      ul->u[8], ul->u[9], ul->u[10], ul->u[11],
      ul->u[12], ul->u[13], ul->u[14], ul->u[15]);

  return ret;
}

MXFUL *
mxf_ul_from_string (const gchar * str, MXFUL * ul)
{
  MXFUL *ret = ul;
  gint len;
  guint i, j;

  g_return_val_if_fail (str != NULL, NULL);

  len = strlen (str);
  if (len != 47) {
    GST_ERROR ("Invalid UL string length %d, should be 47", len);
    return NULL;
  }

  if (ret == NULL)
    ret = g_new0 (MXFUL, 1);

  memset (ret, 0, 16);

  for (i = 0, j = 0; i < 16; i++) {
    if (!g_ascii_isxdigit (str[j]) ||
        !g_ascii_isxdigit (str[j + 1]) ||
        (str[j + 2] != '.' && str[j + 2] != '\0')) {
      GST_ERROR ("Invalid UL string '%s'", str);
      if (ul == NULL)
        g_free (ret);
      return NULL;
    }

    ret->u[i] = (g_ascii_xdigit_value (str[j]) << 4) |
        (g_ascii_xdigit_value (str[j + 1]));
    j += 3;
  }
  return ret;
}

gboolean
mxf_ul_array_parse (MXFUL ** array, guint32 * count, const guint8 * data,
    guint size)
{
  guint32 element_count, element_size;
  guint i;

  g_return_val_if_fail (array != NULL, FALSE);
  g_return_val_if_fail (count != NULL, FALSE);

  if (size < 8)
    return FALSE;

  g_return_val_if_fail (data != NULL, FALSE);

  element_count = GST_READ_UINT32_BE (data);
  data += 4;
  size -= 4;

  if (element_count == 0) {
    *array = NULL;
    *count = 0;
    return TRUE;
  }

  element_size = GST_READ_UINT32_BE (data);
  data += 4;
  size -= 4;

  if (element_size != 16) {
    *array = NULL;
    *count = 0;
    return FALSE;
  }

  if (element_count > size / 16) {
    *array = NULL;
    *count = 0;
    return FALSE;
  }

  *array = g_new (MXFUL, element_count);
  *count = element_count;

  for (i = 0; i < element_count; i++) {
    memcpy (&((*array)[i]), data, 16);
    data += 16;
  }

  return TRUE;
}
