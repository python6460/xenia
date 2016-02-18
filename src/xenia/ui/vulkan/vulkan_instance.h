/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2014 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_UI_VULKAN_VULKAN_INSTANCE_H_
#define XENIA_UI_VULKAN_VULKAN_INSTANCE_H_

#include <memory>
#include <string>
#include <vector>

#include "xenia/ui/vulkan/vulkan.h"
#include "xenia/ui/vulkan/vulkan_util.h"
#include "xenia/ui/window.h"

namespace xe {
namespace ui {
namespace vulkan {

// Wrappers and utilities for VkInstance.
class VulkanInstance {
 public:
  VulkanInstance();
  ~VulkanInstance();

  VkInstance handle = nullptr;

  operator VkInstance() const { return handle; }

  // Declares a layer to verify and enable upon initialization.
  // Must be called before Initialize.
  void DeclareRequiredLayer(std::string name, uint32_t min_version,
                            bool is_optional) {
    required_layers_.push_back({name, min_version, is_optional});
  }

  // Declares an extension to verify and enable upon initialization.
  // Must be called before Initialize.
  void DeclareRequiredExtension(std::string name, uint32_t min_version,
                                bool is_optional) {
    required_extensions_.push_back({name, min_version, is_optional});
  }

  // Initializes the instance, querying and enabling extensions and layers and
  // preparing the instance for general use.
  // If initialization succeeds it's likely that no more failures beyond runtime
  // issues will occur.
  // TODO(benvanik): remove need for any_target_window - it's just for queries.
  bool Initialize(Window* any_target_window);

  // Returns a list of all available devices as detected during initialization.
  const std::vector<DeviceInfo>& available_devices() const {
    return available_devices_;
  }

 private:
  // Queries the system to find global extensions and layers.
  bool QueryGlobals();

  // Creates the instance, enabling required extensions and layers.
  bool CreateInstance();
  void DestroyInstance();

  // Enables debugging info and callbacks for supported layers.
  void EnableDebugValidation();
  void DisableDebugValidation();

  // Queries all available physical devices.
  bool QueryDevices(Window* any_target_window);

  void DumpLayers(const std::vector<LayerInfo>& layers, const char* indent);
  void DumpExtensions(const std::vector<VkExtensionProperties>& extensions,
                      const char* indent);
  void DumpDeviceInfo(const DeviceInfo& device_info);

  std::vector<Requirement> required_layers_;
  std::vector<Requirement> required_extensions_;

  std::vector<LayerInfo> global_layers_;
  std::vector<VkExtensionProperties> global_extensions_;
  std::vector<DeviceInfo> available_devices_;

  VkDebugReportCallbackEXT dbg_report_callback_ = nullptr;
};

}  // namespace vulkan
}  // namespace ui
}  // namespace xe

#endif  // XENIA_UI_VULKAN_VULKAN_INSTANCE_H_
