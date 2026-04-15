//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

class ILineage
{
public:
  virtual ~ILineage() = default;
  [[nodiscard]] virtual const ILineage* parent() const = 0;
};