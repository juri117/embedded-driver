/**
 * @file Sftp.h
 * @author Juri
 * @brief
 * @version 0.1
 * @date 2018-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef DRIVER_DRIVER_PI3_SFTP_H
#define DRIVER_DRIVER_PI3_SFTP_H

#include <stdio.h>
#include <stdlib.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "../libs/ftpclient-cpp/FTP/FTPClient.h"
#include "log.hpp"

// install
// https://github.com/embeddedmz/ftpclient-cpp
// sudo apt-get install libcurl4-openssl-dev

#define PRINT_LOG \
  [](const std::string& strLogMsg) { std::cout << strLogMsg << std::endl; }

using namespace embeddedmz;

class Sftp {
 private:
  std::unique_ptr<CFTPClient> sftp;

 public:
  Sftp();
  bool connect(std::string host, uint16_t port, std::string user,
               std::string pw);
  bool disconnect();
  bool upload_folder(std::string local_path, std::string remote_path,
                     bool delete_after_upload,
                     std::vector<std::string> delte_exceptions);
  bool upload_file(std::string local_path, std::string remote_path,
                   bool delete_after_upload);
  bool download_file(std::string local_path, std::string remote_path);
};

#endif