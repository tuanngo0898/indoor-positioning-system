/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @file ccp_json.h
 * @author UWB Core <uwbcore@gmail.com>
 * @date  Apr 10 2020
 * @brief
 *
 */

#ifndef _CCP_JSON_H_
#define _CCP_JSON_H_

#include <json/json.h>
#include <json/json_util.h>
#include <dpl/dpl.h>
#include <dpl/dpl_types.h>

typedef struct _ccp_json{
    /* json_decoder must be first element in the structure */
    struct json_decoder decoder;
    struct json_encoder encoder;
    json_status_t status;
    uint64_t utime;
    uint8_t seq;
    uint32_t uid;
    union {
        dpl_float64_t ccp[2];
        uint64_t ccp_uint64[2];
    };
    uint64_t count;
    union {
        dpl_float64_t ppm;
        uint64_t ppm_uint64;
    };
    char iobuf[MYNEWT_VAL(UWB_CCP_JSON_BUFSIZE)];
    uint16_t idx;
}ccp_json_t;

ccp_json_t * ccp_json_init(ccp_json_t * json);
void ccp_json_free(ccp_json_t * json);
#ifdef FLOAT_SUPPORT
int ccp_json_read(ccp_json_t * json, char * line);
#endif
int ccp_json_read_uint64(ccp_json_t * json, char * line);
int ccp_json_write(ccp_json_t * json);
int ccp_json_write_uint64(ccp_json_t * json);

#endif
