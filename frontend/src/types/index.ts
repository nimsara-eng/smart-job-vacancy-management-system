// ─── Models (mirrors C++ structs) ──────────────────────────────────────────

export interface Job {
  id: number
  title: string
  company: string
  requiredSkills: string[]
  salary: number
  isOpen: boolean
  isRemote: boolean
  description: string
  location: string
  category: string
}

export interface User {
  id: number
  username: string
  password: string
  email: string
  skills: string[]
  role: 'applicant' | 'employer'
  name: string
  phone: string
}

export interface Application {
  applicationId: number
  userId: number
  jobId: number
  status: 'pending' | 'approved' | 'rejected'
  appliedDate: string
}

// ─── Auth State ─────────────────────────────────────────────────────────────

export interface AuthState {
  user: User | null
  isLoggedIn: boolean
}