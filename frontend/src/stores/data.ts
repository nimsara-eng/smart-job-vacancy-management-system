import { defineStore } from 'pinia'
import { ref } from 'vue'
import type { Job, User, Application } from '../types'

// ─── Helpers: read / write JSON files via fetch ─────────────────────────────
// In dev the data/ folder is served by Vite from the project root.
// Writes are simulated (state only); for real persistence wire up a small
// Express bridge or Tauri/Electron backend that calls your C++ .exe.

async function loadJSON<T>(path: string): Promise<T> {
  const res = await fetch(path)
  if (!res.ok) throw new Error(`Failed to load ${path}`)
  return res.json()
}

export const useDataStore = defineStore('data', () => {
  const jobs = ref<Job[]>([])
  const users = ref<User[]>([])
  const applications = ref<Application[]>([])
  const loading = ref(false)
  const error = ref<string | null>(null)

  // ── Load ──────────────────────────────────────────────────────────────────
  async function loadAll() {
    loading.value = true
    error.value = null
    try {
      ;[jobs.value, users.value, applications.value] = await Promise.all([
        loadJSON<Job[]>('/data/jobs.json'),
        loadJSON<User[]>('/data/users.json'),
        loadJSON<Application[]>('/data/applications.json'),
      ])
    } catch (e: any) {
      error.value = e.message
    } finally {
      loading.value = false
    }
  }

  // ── Jobs ──────────────────────────────────────────────────────────────────
  function addJob(job: Omit<Job, 'id'>) {
    const id = jobs.value.length ? Math.max(...jobs.value.map(j => j.id)) + 1 : 1
    jobs.value.push({ ...job, id })
  }

  function updateJob(updated: Job) {
    const idx = jobs.value.findIndex(j => j.id === updated.id)
    if (idx !== -1) jobs.value[idx] = updated
  }

  function removeJob(id: number) {
    jobs.value = jobs.value.filter(j => j.id !== id)
  }

  function toggleJobStatus(id: number) {
    const job = jobs.value.find(j => j.id === id)
    if (job) job.isOpen = !job.isOpen
  }

  function getJobById(id: number) {
    return jobs.value.find(j => j.id === id) ?? null
  }

  function searchJobs(query: string, category: string, remote: boolean | null) {
    const q = query.toLowerCase()
    return jobs.value.filter(j => {
      const matchQuery =
        !q ||
        j.title.toLowerCase().includes(q) ||
        j.company.toLowerCase().includes(q) ||
        j.location.toLowerCase().includes(q) ||
        j.requiredSkills.some(s => s.toLowerCase().includes(q))
      const matchCat = !category || j.category === category
      const matchRemote = remote === null || j.isRemote === remote
      return matchQuery && matchCat && matchRemote
    })
  }

  function getMatchingJobsForUser(userSkills: string[]) {
    return jobs.value
      .filter(j => j.isOpen)
      .map(j => {
        const matched = j.requiredSkills.filter(s =>
          userSkills.map(x => x.toLowerCase()).includes(s.toLowerCase()),
        )
        return { job: j, matchScore: matched.length / Math.max(j.requiredSkills.length, 1) }
      })
      .filter(x => x.matchScore > 0)
      .sort((a, b) => b.matchScore - a.matchScore)
  }

  // ── Users ─────────────────────────────────────────────────────────────────
  function registerUser(user: Omit<User, 'id'>) {
    if (users.value.some(u => u.username === user.username)) {
      throw new Error('Username already exists')
    }
    const id = users.value.length ? Math.max(...users.value.map(u => u.id)) + 1 : 1
    const newUser: User = { ...user, id }
    users.value.push(newUser)
    return newUser
  }

  function loginUser(username: string, password: string): User | null {
    return users.value.find(u => u.username === username && u.password === password) ?? null
  }

  function updateUser(updated: User) {
    const idx = users.value.findIndex(u => u.id === updated.id)
    if (idx !== -1) users.value[idx] = updated
  }

  // ── Applications ──────────────────────────────────────────────────────────
  function submitApplication(app: Omit<Application, 'applicationId'>) {
    const applicationId = applications.value.length
      ? Math.max(...applications.value.map(a => a.applicationId)) + 1
      : 1
    applications.value.push({ ...app, applicationId })
  }

  function updateApplicationStatus(applicationId: number, status: Application['status']) {
    const app = applications.value.find(a => a.applicationId === applicationId)
    if (app) app.status = status
  }

  function getApplicationsByUser(userId: number) {
    return applications.value.filter(a => a.userId === userId)
  }

  function getApplicationsByJob(jobId: number) {
    return applications.value.filter(a => a.jobId === jobId)
  }

  function hasApplied(userId: number, jobId: number) {
    return applications.value.some(a => a.userId === userId && a.jobId === jobId)
  }

  return {
    jobs,
    users,
    applications,
    loading,
    error,
    loadAll,
    addJob,
    updateJob,
    removeJob,
    toggleJobStatus,
    getJobById,
    searchJobs,
    getMatchingJobsForUser,
    registerUser,
    loginUser,
    updateUser,
    submitApplication,
    updateApplicationStatus,
    getApplicationsByUser,
    getApplicationsByJob,
    hasApplied,
  }
})