/// Author: Theodore Abohweyere

#include <cstdint>

typedef struct
{
    /* @details Two bits for the leap indicator.
     * Three bits for the version number of the protocol
     * Three bits for the Mode. Pick value of 3 for client mode
     */
    uint8_t leapVersionNumberMode = 0;

    /// @brief Stratum level of the local clock
    uint8_t stratum = 0;

    /// @brief Maximum interval between successive messages
    uint8_t poll = 0;

    /// @brief Precision of the local clock
    uint8_t precision = 0;

    /// @brief Total round trip delay time
    uint32_t rootDelay = 0;

    /// @brief Max error allowed from primary clock source
    uint32_t rootDispersion = 0;

    /// @brief Reference clock identifier
    uint32_t referenceClockId = 0;

    /// @brief Reference time stamp in seconds
    uint32_t referenceTimeStampSeconds = 0;

    /// @brief Reference time stamp fraction of a second
    uint32_t referenceTimeStampFraction = 0;

    /// @brief Originate time stamp in seconds
    uint32_t originateTimeStampSeconds = 0;

    /// @brief Originate time stamp fraction of a second
    uint32_t originateTimeStampFraction = 0;

    /// @brief Receiving time stamp in seconds. The time the packet was received by the client
    uint32_t receivingTimeStampSeconds = 0;

    /// @brief Receiving time stamp fraction of a second
    uint32_t receivingTimeStampFraction = 0;

    /// @brief Transmit time stamp in seconds. The time the packet left the server
    uint32_t transmitTimeStampSeconds = 0;

    /// @brief Transmit time stamp fraction of a second
    uint32_t transmitTimeStampFraction = 0;
} NtpPacket;

/// @brief Extract leap identifier from the first two bits of [leapVersionNumberMode]
/// @param leapVersionNumberMode
/// @return
uint8_t extractLeapIdentifier(uint8_t leapVersionNumberMode)
{
    return leapVersionNumberMode >> 6;
}

/// @brief Returns a new value with the leap identifier bits set to the last two bits of [leapIdentifier]
/// @param leapIdentifier
/// @param leapVersionNumberMode
/// @return
uint8_t setLeapIdentifier(uint8_t leapIdentifier, uint8_t leapVersionNumberMode)
{
    uint8_t cleaned = leapIdentifier & (uint8_t)0b00000011; // Clear all bits except the last 2 bits
    return (leapIdentifier << 6) | (leapVersionNumberMode & 0b00111111);
}

/// @brief Extract version number from the three bits after the leap identifier in [leapVersionNumberMode]
/// @param leapVersionNumberMode
/// @return
uint8_t extractVersionNumber(uint8_t leapVersionNumberMode)
{
    uint8_t withVersionNumberAsLastBits = leapVersionNumberMode >> 3;          // Make the version number the last 3 bits
    uint8_t versionNumber = withVersionNumberAsLastBits & (uint8_t)0b00000111; // Clear all bits except the last 3 bits

    return versionNumber;
}

/// @brief Returns a new value with the version number bits set to the last three bits of [versionNumber]
/// @param versionNumber
/// @param leapVersionNumberMode
/// @return
uint8_t setVersionNumber(uint8_t versionNumber, uint8_t leapVersionNumberMode)
{
    uint8_t cleaned = versionNumber & (uint8_t)0b00000111; // Clear all bits except the last 3 bits
    return (cleaned << 3) | (leapVersionNumberMode & 0b11000111);
}

/// @brief Extract mode from the last three bits of [leapVersionNumberMode]
/// @param leapVersionNumberMode
/// @return
uint8_t extractMode(uint8_t leapVersionNumberMode)
{
    uint8_t mode = leapVersionNumberMode & 0b00000111; // Clear all bits except the last 3 bits

    return mode;
}

/// @brief Returns a new value with the mode bits set to the last three bits of [versionNumber]
/// @param versionNumber
/// @param leapVersionNumberMode
/// @return
uint8_t setMode(uint8_t mode, uint8_t leapVersionNumberMode)
{
    uint8_t cleaned = mode & (uint8_t)0b00000111; // Clear all bits except the last 3 bits

    return cleaned | (leapVersionNumberMode & 0b11111000);
}
