#
# Be sure to run `pod lib lint MisfitSleepSDK.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
s.name             = "MisfitSleepSDK"
s.version          = "0.1.0"
s.summary          = "Gain access to user sleep events from the Misfit Shine 2 and Flash"

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!
s.description      = <<-DESC
The Misfit Sleep SDK tracks users’ sleep sessions and can notify your application when they fall asleep as well as when they should be woken up.
DESC

s.homepage         = "https://github.com/Misfit-Developers/SleepSDK.git"
# s.screenshots     = "www.example.com/screenshots_1", "www.example.com/screenshots_2"
s.license          = 'MIT'
s.author           = { "Phill Pasqual" => "phill@misfit.com" }
s.source           = { :git => "https://github.com/Misfit-Developers/SleepSDK.git", :tag => s.version.to_s }

s.platform     = :ios, '7.0'
s.requires_arc = true

s.preserve_paths = "Pod/MisfitSleepSDK.framework"
s.vendored_frameworks = "Pod/MisfitSleepSDK.framework"
s.public_header_files = "Pod/MisfitSleepSDK.framework/Versions/A/Headers/*.h"
s.frameworks = 'CoreBluetooth'
# s.dependency 'AFNetworking', '~> 2.3'
end
